#include <set>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

using point = std::pair<int, int>;
using fold = std::pair<char, int>;

void print_map(const auto& pmap) {
    int x_max = 0, y_max = 0;
    for(const auto& [px, py]: pmap) {
        x_max = std::max(x_max, px);
        y_max = std::max(y_max, py);
    }
    for(int y = 0; y <= y_max; y++) {
        for(int x = 0; x <= x_max; x++) {
            std::cout << (pmap.contains(point{x,y}) ? '#' : ' ');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//842
//BFKRCJZU
int main() {
    std::set<point> point_map;
    std::list<fold> folds;

    if(std::ifstream input_file("input"); input_file.is_open()) {
        for(std::string line_str; std::getline(input_file, line_str, '\n');) {
            if(const auto sep_pos = line_str.find_first_of(",="); sep_pos != line_str.npos) {
                if(line_str[sep_pos] == ',')
                    point_map.insert(point{std::stoi(line_str.substr(0ULL, sep_pos)), std::stoi(line_str.substr(sep_pos+1))});
                else
                    folds.push_back(fold{line_str[sep_pos-1], std::stoi(line_str.substr(sep_pos + 1))});
            }
        }
    }

    for(const auto [fold_dir, fold_pos] : folds) {
        std::set<point> folded_map;
        for (const auto [px, py] : point_map)
            folded_map.emplace(
                (fold_dir == 'x' && px > fold_pos) ? 2*fold_pos - px : px,
                (fold_dir == 'y' && py > fold_pos) ? 2*fold_pos - py : py
            );

        point_map = folded_map;

        for(static bool part1 = true; part1; part1 = false)
            std::cout << point_map.size() << std::endl;
    }
    print_map(point_map);
    return 0;
}