#include <map>
#include <limits>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

using namespace std::string_view_literals;

using freq_map_t = std::map<char, unsigned long long>;
using memo_t = std::map<std::tuple<int, char, char>, freq_map_t>;

freq_map_t merge_fmaps(freq_map_t m1, const freq_map_t& m2, const char common_ch) {
    for(const auto [ch, val] : m2) m1[ch] += val - (ch == common_ch);
    return m1;
}

freq_map_t expand(memo_t& memo, const auto& poly_template, int level, const char p0, const char p1) {
    if(level == 0) return (p0 != p1) ? freq_map_t{{p0, 1}, {p1, 1}} : freq_map_t{{p0, 2}};
    
    const auto memo_key = std::tuple{level, p0, p1};
    if(memo.contains(memo_key)) return memo.at(memo_key);
    
    const auto x = poly_template.at(std::string() + p0 + p1);
    return memo[memo_key] = merge_fmaps(
        expand(memo, poly_template, level - 1, p0, x),
        expand(memo, poly_template, level - 1, x, p1),
        x
    );
}

//3306
//3760312702877
int main() {
    const auto [poly_string, poly_template] = []() {
        std::string poly_string;
        std::map<std::string, char> poly_template;

        if(std::ifstream input_file("input"); input_file.is_open()) {
            for(std::string line_str; std::getline(input_file, line_str, '\n');) {
                if(line_str.length() == 0) continue;
                if(const auto sep_pos = line_str.find(" -> "sv); sep_pos != line_str.npos) {
                    const auto input = line_str.substr(0ULL, sep_pos);
                    const auto output = line_str.substr(sep_pos + " -> "sv.length());
                    if(input.length() != 2 || output.length() != 1) throw;
                    poly_template.emplace(input, output[0]);
                } else {
                    if(poly_string.length() != 0) throw;
                    poly_string = line_str;
                };
            }
            return std::pair{poly_string, poly_template};
        } else throw;
    }();

    memo_t memo;
    const auto get_diff = [&](const int levels) -> unsigned long long {
        freq_map_t freq_map = expand(memo, poly_template, levels, poly_string[0], poly_string[1]);
        for(size_t i = 1; i < poly_string.length() - 1; i++) {
            freq_map = merge_fmaps(freq_map, expand(memo, poly_template, levels, poly_string[i], poly_string[i+1]), poly_string[i]);
        }
        auto min = std::numeric_limits<unsigned long long>::max();
        auto max = std::numeric_limits<unsigned long long>::min();
        for(const auto [ch, val] : freq_map) {
            min = std::min(min, val);
            max = std::max(max, val);
        }
        return max - min;
    };

    std::cout << get_diff(10) << std::endl;
    std::cout << get_diff(40) << std::endl;

    return 0;
}