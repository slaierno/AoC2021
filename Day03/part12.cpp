#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

// 749376
// 2372923
int main()
{
    std::vector<unsigned long long> input;
    size_t bin_size;
    if(std::ifstream input_file("input"); input_file.is_open()) {
        /* O(n) */
        for(std::string num_str; std::getline(input_file, num_str, '\n');) {
            input.push_back(std::stoull(num_str, 0, 2));
            if(bin_size == 0) bin_size = num_str.length();
        }
    }
    {
        std::vector<int> bit_count(bin_size, 0);
        for(const auto bin : input) {
            for(size_t i = 0; i < bin_size; i++) {
                const auto mask = 1 << i;
                bit_count[i] += (bin & mask) ? 1 : -1;
            }
        }
        unsigned gamma = 0;
        for(size_t i = 0; i < bin_size; i++) {
            if (bit_count[i] > 0) gamma |= 1 << i;
        }
        std::cout << gamma * ((1 << bin_size) - gamma - 1) << std::endl;
    }
    {
        auto ox_filter = input, co2_filter = input;
        for(size_t bit = bin_size-1; ox_filter.size() > 1; bit--) {
            int bit_count = 0;
            const auto mask = 1 << bit;
            std::vector<unsigned long long> filter[2];
            for(const auto el : ox_filter) {
                filter[!!(mask & el)].push_back(el);
                bit_count += (mask & el) ? 1 : -1;
            }
            ox_filter = filter[bit_count >= 0];
        }
        for(size_t bit = bin_size-1; co2_filter.size() > 1; bit--) {
            int bit_count = 0;
            const auto mask = 1 << bit;
            std::vector<unsigned long long> filter[2];
            for(const auto el : co2_filter) {
                filter[!!(mask & el)].push_back(el);
                bit_count += (mask & el) ? 1 : -1;
            }
            co2_filter = filter[!(bit_count >= 0)];
        }
        std::cout << co2_filter.back() * ox_filter.back() << std::endl;
    }
    return 0;
}