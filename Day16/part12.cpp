#include <vector>
#include <numeric>
#include <concepts>
#include <string>
#include <variant>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <fstream>

//1038
//246761930504

using literal_t = __uint128_t;

class bitvec_t {
    std::vector<bool> m_vec;
    size_t m_ptr = 0;

    bool peek() const { return m_vec[m_ptr]; }
public:
    size_t ptr() const { return m_ptr; }

    void add_num(const std::string& num_str) {
        if(num_str.length() > 16) throw std::runtime_error("num_str.length() > 16");
        auto num = std::stoull(num_str, nullptr, 16) << (16 - num_str.length()) * 4;
        const size_t bit_size = num_str.length() * 4;
        m_vec.reserve(m_vec.size() + bit_size);
        for(size_t i = 0; i < bit_size; i++, num <<= 1) {
            m_vec.push_back(num >> (sizeof(num)*8 - 1));
        }
    }

    template<size_t bits>
    unsigned get_num() {
        static_assert(bits <= sizeof(unsigned) * 8);
        if(bits + m_ptr > m_vec.size()) throw std::runtime_error("Trying to read outside bitstring");
        unsigned ret = 0;
        for(size_t i = 0; i < bits; i++) {
            ret = ret << 1 | m_vec[m_ptr++];
        }
        return ret;
    }

    literal_t get_literal() {
        literal_t ret = 0;
        size_t byte_cnt = 0;
        while(true) {
            if(++byte_cnt > 16) throw std::runtime_error("trying to read an integer greater than 128 bit");
            const uint8_t n = get_num<5>();
            ret = (ret << 4) | (n & 0xF);
            if(n >> 4 == 0) return ret;
        }
    }
};

struct packet;

class packet {
    struct operator_info {
        bool length_type_id;
        uint16_t length; // ltid = 0 => 15 bit packets length
                        // ltid = 1 => 11 bit packet count
        std::vector<packet> packets;
    };

    std::variant<literal_t, operator_info> info;
    uint8_t version_id;
    uint8_t type_id;
    literal_t result;

public:
    auto operator<=>(const packet& oth) const { return result <=> oth.result; }
    operator literal_t() const { return result; }
    packet(bitvec_t& input) : version_id(input.get_num<3>()), type_id (input.get_num<3>()) {
        if(type_id == 4) {
            result = input.get_literal();
            info = result;
        } else {
            info = operator_info();
            auto& op_info = std::get<operator_info>(info);
            if((op_info.length_type_id = input.get_num<1>())) {
                op_info.length = input.get_num<11>();
                op_info.packets.reserve(op_info.length);
                for(size_t i = 0; i < op_info.length; i++) {
                    op_info.packets.push_back(packet(input));
                }
            } else {
                op_info.length = input.get_num<15>();
                auto start_ptr = input.ptr();
                while(input.ptr() < start_ptr + op_info.length) {
                    op_info.packets.push_back(packet(input));
                }
            }
            switch(type_id) {
            case 0:
                result = std::accumulate(op_info.packets.begin(), op_info.packets.end(), (literal_t)(0));
                break;
            case 1:
                result = std::accumulate(op_info.packets.begin(), op_info.packets.end(), (literal_t)(1), std::multiplies<literal_t>{});
                break;
            case 2:
                result = std::ranges::min_element(op_info.packets)->result;
                break;
            case 3:
                result = std::ranges::max_element(op_info.packets)->result;
                break;
            case 5:
                if(op_info.packets.size() != 2) throw std::runtime_error("Expected 2 packets for operator 5");
                result = op_info.packets[0].result > op_info.packets[1].result;
                break;
            case 6:
                if(op_info.packets.size() != 2) throw std::runtime_error("Expected 2 packets for operator 6");
                result = op_info.packets[0].result < op_info.packets[1].result;
                break;
            case 7:
                if(op_info.packets.size() != 2) throw std::runtime_error("Expected 2 packets for operator 7");
                result = op_info.packets[0].result == op_info.packets[1].result;
                break;
            default: throw std::runtime_error("Unexpected type id");
            }
        }
    }

    uint64_t get_sum_of_versions() const {
        uint64_t ret = version_id;
        if(std::holds_alternative<operator_info>(info)) {
            const auto& op_info = std::get<operator_info>(info);
            ret += std::accumulate(op_info.packets.begin(), op_info.packets.end(), 0ULL, [](const auto acc, const auto p) {return acc + p.get_sum_of_versions(); });
        }
        return ret;
    }

    auto get_result() const { return result; }
};

std::ostream &operator<<(std::ostream &dest, __uint128_t value) {
    std::ostream::sentry s(dest);
    if(s) {
        char buffer[128], *d = std::end(buffer);
        do {
            *(--d) = "0123456789"[value % 10];
            value /= 10;
        } while (value != 0);
        if(int len = std::end(buffer) - d; dest.rdbuf()->sputn(d, len) != len)
            dest.setstate(std::ios_base::badbit);
    }
    return dest;
}

int main() {
    bitvec_t input;
    if(std::ifstream input_file("input"); input_file.is_open()) {
        std::string num_str;
        std::getline(input_file, num_str, '\n');
        for(size_t i = 0; i < num_str.length(); i+=16)
            input.add_num(num_str.substr(i, 16));
    }
    auto p = packet(input);
    std::cout << p.get_sum_of_versions() << std::endl;
    std::cout << p.get_result() << std::endl;
    return 0;
}