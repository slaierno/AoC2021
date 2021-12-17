#include <array>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <iostream>
#include <map>
#include <utils.hpp>

//1027422
//01:43:45
struct num_display {
    std::array<bool, 7> sev_seg {};
    num_display() = default;
    num_display(std::string_view d) { std::for_each(d.begin(), d.end(), [this](const auto ch) {sev_seg[ch - 'a'] = true;}); }
    auto operator<=>(const num_display&) const = default;
    auto on_count() const { return std::count(sev_seg.begin(), sev_seg.end(), true); }
    auto intersect_count(const num_display &oth) const {
        int count = 0;
        for (size_t i = 0; i < sev_seg.size(); i++)
            count += sev_seg[i] && oth.sev_seg[i];
        return count;
    }
    auto contains(const num_display &oth) const { return intersect_count(oth) == oth.on_count(); }
};

struct decoder {
    std::array<num_display, 10> unique_patterns;
    std::array<num_display, 4> now_displaying;
    std::map<num_display, int> p_num_mapping;
    std::map<int, num_display> num_p_mapping;

    decoder(std::vector<std::vector<num_display>> input) {
        if(input.size() != 2 || input[0].size() != 10 || input[1].size() != 4) throw;
        std::copy(input[0].begin(), input[0].end(), unique_patterns.begin());
        std::copy(input[1].begin(), input[1].end(), now_displaying.begin());
        const auto conditional_mapping = [this](auto count, auto num, auto lambda) {
            const auto f = std::find_if(unique_patterns.begin(), unique_patterns.end(), [this, lambda, count](const auto& p) { return p_num_mapping.find(p) == p_num_mapping.end() && p.on_count() == count && lambda(p);});
            p_num_mapping[*f] = num;
            num_p_mapping[num] = *f;
        };
        const auto direct_mapping = [this, conditional_mapping](auto count, auto num) {
            return conditional_mapping(count, num, []([[maybe_unused]] const auto& p) {return true;});
        };
        direct_mapping(2, 1);
        direct_mapping(3, 7);
        direct_mapping(4, 4);
        direct_mapping(7, 8);
        conditional_mapping(5, 3, [this](const auto& p) { return p.contains(num_p_mapping[1]); });
        conditional_mapping(5, 5, [this](const auto& p) { return p.intersect_count(num_p_mapping[4]) == 3; });
        direct_mapping(5, 2);
        conditional_mapping(6, 9, [this](const auto& p) { return p.contains(num_p_mapping[4]); });
        conditional_mapping(6, 0, [this](const auto& p) { return p.contains(num_p_mapping[7]); });
        direct_mapping(6, 6);
    }

    auto sum_of_shown() const {
        return p_num_mapping.find(now_displaying[0])->second * 1000 + 
               p_num_mapping.find(now_displaying[1])->second * 100 + 
               p_num_mapping.find(now_displaying[2])->second * 10 + 
               p_num_mapping.find(now_displaying[3])->second;
    }
};

int main() {
    const auto input = AoC::get_input("input", '\n', [](const auto& display) {
        return AoC::split(display, " | ", [](const auto& patterns) {
            return AoC::split(patterns, ' ', [](const auto& pattern) {
                return num_display(pattern);
            });
        });
    });
    const auto sum_of_display = std::accumulate(input.begin(), input.end(), 0, [](const auto acc, const auto& display) { return acc + decoder(display).sum_of_shown(); });
    std::cout << sum_of_display << std::endl;
    return 0;
}