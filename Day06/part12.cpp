#include <array>
#include <iostream>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>

//377263
//1695929023803
int main() {
    using namespace ranges;
    constexpr std::array<int, 300> fishes {
        5,1,1,5,4,2,1,2,1,2,2,1,1,1,4,2,2,4,1,1,1,1,1,4,1,1,1,1,1,5,3,1,4,1,1,1,1,1,4,1,5,1,1,1,4,1,2,2,3,1,5,1,1,5,1,1,5,4,1,1,1,4,3,1,1,1,3,1,5,5,1,1,1,1,5,3,2,1,2,3,1,5,1,1,4,1,1,2,1,5,1,1,1,1,5,4,5,1,3,1,3,3,5,5,1,3,1,5,3,1,1,4,2,3,3,1,2,4,1,1,1,1,1,1,1,2,1,1,4,1,3,2,5,2,1,1,1,4,2,1,1,1,4,2,4,1,1,1,1,4,1,3,5,5,1,2,1,3,1,1,4,1,1,1,1,2,1,1,4,2,3,1,1,1,1,1,1,1,4,5,1,1,3,1,1,2,1,1,1,5,1,1,1,1,1,3,2,1,2,4,5,1,5,4,1,1,3,1,1,5,5,1,3,1,1,1,1,4,4,2,1,2,1,1,5,1,1,4,5,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,4,2,1,1,1,2,5,1,4,1,1,1,4,1,1,5,4,4,3,1,1,4,5,1,1,3,5,3,1,2,5,3,4,1,3,5,4,1,3,1,5,1,4,1,1,4,2,1,1,1,3,2,1,1,4
    };
    std::array<size_t, 9> age_groups {};
    for_each(fishes, [&](auto age) { age_groups[age]++; });
    for(int i = 0; i < 256; i++) {
        const std::array<size_t, 9> temp_ag = age_groups;
        for_each(views::iota(0,9), [&](int i) { age_groups[i] = temp_ag[i == 8 ? 0 : i + 1] + (i == 6 ? temp_ag[0] : 0); });
        if (i == 79 || i == 255) std::cout << accumulate(age_groups, 0ULL) << std::endl;
    }
}