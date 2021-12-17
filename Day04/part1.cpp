#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <map>
#include <utils.hpp>

// 49686

struct matrix {
    using num_t = unsigned long long;
    using row = std::vector<num_t>;
    std::vector<row> rows;
    std::map<num_t, std::pair<size_t, size_t>> num_map;
    std::vector<std::vector<bool>> pins;
    matrix(auto r) : rows(r), pins(std::vector<std::vector<bool>>(r.size(), std::vector<bool>(r.back().size(), false))) {
        for(size_t i = 0; i < rows.size(); i++) {
            for(size_t j = 0; j < rows[i].size(); j++) {
                num_map[rows[i][j]] = {i, j};
            }
        }
    }

    bool search_row(size_t row_idx) const {
        return std::all_of(pins[row_idx].begin(), pins[row_idx].end(), [](auto x) {return x;});
    }

    bool search_col(size_t col_idx) const {
        for(size_t j = 0; j < rows.size(); j++) {
            if (!pins[j][col_idx]) return false;
        }
        return true;
    }

    bool pin(const num_t n) {
        if(const auto search = num_map.find(n); search != num_map.end()) {
            const auto [i, j] = search->second;
            pins[i][j] = true;
            return search_row(i) || search_col(j);
        } else return false;
    }

    num_t uncalled_sum() const {
        return std::accumulate(num_map.begin(), num_map.end(), (num_t) 0, [this](num_t acc, auto n_entry) {
            const auto& [n, p] = n_entry;
            const auto& [i, j] = p;
            return acc + ((pins[i][j]) ? 0 : n);
        });
    }
};

int main() {
    const auto input = AoC::get_input("input", "\n\n");
    const auto bingo_nums = AoC::split(input[0], ',', [](auto n){return std::stoull(n);});
    auto boards = AoC::for_each_ret(std::next(input.begin()), input.end(),
        [](const auto board) -> matrix {
            return AoC::split(board, '\n', [](const auto row_str) {
                return AoC::split(row_str, ' ', [](const auto el) { return std::stoull(el); });
            });
        });
    std::any_of(bingo_nums.begin(), bingo_nums.end(), [&boards](const auto num) {
        if (auto it = std::find_if(boards.begin(), boards.end(), [num](auto& board) {
            return board.pin(num);
        }); it != boards.end()) {
            std::cout << num * it->uncalled_sum() << std::endl;
            return true;
        } else {
            return false;
        }
    });
    return 0;
}