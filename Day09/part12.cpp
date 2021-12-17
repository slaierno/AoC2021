#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <string>
#include <fstream>
#include <iostream>

#include <range/v3/all.hpp>

//498
//1071000

using point = std::tuple<size_t, size_t>;

class heightmap {
    std::vector<std::vector<int>> m_map;
public:
    heightmap(auto input_filename) {
        if(std::ifstream input_file(input_filename); input_file.is_open()) {
            for(std::string num_str; std::getline(input_file, num_str, '\n');) {
                m_map.emplace_back();
                for(const auto ch : num_str) {
                    m_map.back().push_back(ch - '0');
                }
            }
        } else throw;
    }

    auto get(auto p) const { return m_map.at(std::get<0>(p)).at(std::get<1>(p)); }
    auto n_rows() const { return m_map.size(); }
    auto n_cols() const { return m_map.back().size(); }
    auto get_neighs(const auto p) const {
        const auto [i, j] = p;
        std::vector<point> neighs {};
        if(j > 0) neighs.emplace_back(i  , j-1);
        if(i > 0) neighs.emplace_back(i-1, j  );
        if(j < n_cols() - 1) neighs.emplace_back(i  , j+1);
        if(i < n_rows() - 1) neighs.emplace_back(i+1, j  );
        return neighs;
    }

    auto indexes_view() const {
        return ranges::views::cartesian_product(ranges::views::ints(0UL, n_rows()), ranges::views::ints(0UL, n_cols()));
    }
};

int main() {
    const heightmap input("input");
    int local_minima_sum = 0;
    std::set<size_t> basin_sizes {};
    for(const auto p : input.indexes_view()) {
        if(const auto min = input.get(p); min < 9 && std::ranges::all_of(input.get_neighs(p), [&input, min](const auto n){return input.get(n) > min;})) {
            local_minima_sum += min + 1;
            std::set<point> basin {};
            for(auto [to_explore, el] = std::pair{std::set<point>{p}, p}; !to_explore.empty(); el = *to_explore.begin()) {
                basin.emplace(el);
                to_explore.erase(el);
                for(const auto& n : input.get_neighs(el)) {
                    if(input.get(n) < 9
                    && input.get(n) > input.get(el)
                    && basin.find(n) == basin.end()
                    && to_explore.find(n) == to_explore.end())
                        to_explore.emplace(n);
                }
            }
            basin_sizes.emplace(basin.size());
        }
    }
    std::cout << local_minima_sum << std::endl;
    std::cout << ranges::accumulate(basin_sizes.rbegin(), std::next(basin_sizes.rbegin(), 3), 1, std::multiplies<size_t>{}) << std::endl;
}