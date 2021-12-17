#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

//1226
//1252
int main()
{
    std::vector<int> input;
    if(std::ifstream input_file("input"); input_file.is_open()) {
        /* O(n) */
        for(std::string num_str; std::getline(input_file, num_str, '\n');) {
            input.push_back(std::stoull(num_str));
        }
    }
    std::vector<int> diff;
    std::vector<int> sums;
    std::adjacent_difference(input.begin(), input.end(), std::back_inserter(diff));
    std::cout << std::count_if(std::next(diff.begin()), diff.end(), [](auto x){return x > 0;}) << std::endl;
    std::adjacent_difference(input.begin(), input.end(), std::back_inserter(sums), std::plus<>{});
    std::transform(std::next(sums.begin(),2), sums.end(), input.begin(), std::next(sums.begin(),2), std::plus<>{});
    std::adjacent_difference(sums.begin(), sums.end(), diff.begin());
    std::cout << std::count_if(std::next(diff.begin(), 3), diff.end(), [](auto x){return x > 0;}) << std::endl;
    return 0;
}