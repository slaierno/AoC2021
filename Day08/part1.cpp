#include <array>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <iostream>
#include <utils.hpp>

//397
//24:48
struct num_display {
    std::array<bool, 7> sev_seg {};
    num_display(std::string_view d) {
        std::for_each(d.begin(), d.end(), [this](const auto ch) {sev_seg[ch - 'a'] = true;});
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
    const auto cnt_1478 = std::accumulate(input.begin(), input.end(), 0, [](const auto acc, const auto& display) {
        const auto& shown = display[1];
        return acc + std::count_if(shown.begin(), shown.end(), [](const auto& sseg){
            const auto on_count = std::count(sseg.sev_seg.begin(), sseg.sev_seg.end(), true);
            return on_count == 2 || on_count == 3 || on_count == 4 || on_count == 7;
        });
    });
    std::cout << cnt_1478 << std::endl;
    return 0;
}