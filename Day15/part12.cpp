#include <vector>
#include <map>
#include <string>
#include <limits>
#include <iostream>
#include <fstream>

using dist_t = unsigned long long;
using point_t = std::pair<size_t, size_t>;

//581
//2916
void execute(const int repeat) {
    const auto [input, size] = [=](){
        std::vector<dist_t> input;
        size_t size = 0;
        for(int i = 0; i < repeat; i++) {
            if(std::ifstream input_file("input"); input_file.is_open()) {
                for(std::string num_str; std::getline(input_file, num_str, '\n');) {
                    if(!size) size = num_str.length();
                    for(int j = 0; j < repeat; j++)
                        std::transform(std::begin(num_str), std::end(num_str), std::back_inserter(input), [n=i+j](const auto ch) {return ((ch-'1' + n) % 9) + 1;});
                }
            }
        }
        return std::pair{input, size*repeat};
    }();

    const auto to_idx = [size](const auto p) { const auto [x, y] = p; return x + y * size; };
    const auto square_cost = [&](const auto p) {
        return input[to_idx(p)];
    };
    std::vector<dist_t> m_dist(size*size, std::numeric_limits<dist_t>::max());
    const auto& dist = [&to_idx, &m_dist](const auto p) -> dist_t& {
        return m_dist[to_idx(p)];
    };
    std::vector<bool> m_visited(size*size, false);
    const auto& visited = [&to_idx, &m_visited](const auto p) {
        return m_visited[to_idx(p)];
    };
    std::multimap<dist_t, point_t> unvisited_map;
    const auto update_unvisited = [&unvisited_map, &dist](const auto p, const auto new_dist){
        const auto [erb, ere] = unvisited_map.equal_range(dist(p));
        const auto del_it = std::find_if(erb, ere, [p](const auto& kv_pair) { return kv_pair.second == p; });
        if (del_it != ere) unvisited_map.erase(del_it);
        dist(p) = new_dist;
        unvisited_map.emplace(new_dist, p);
    };
    const auto get_best_unvisited = [&unvisited_map]() {
        return unvisited_map.extract(unvisited_map.begin()).mapped();
    };

    const auto get_neighs_not_visited = [size, &visited, &to_idx](const auto p) {
        const auto& [x, y] = p;
        std::vector<point_t> n_list;
        if(const auto p1 = point_t{x-1, y  }; x > 0               && !visited(p1)) n_list.push_back(p1);
        if(const auto p1 = point_t{x  , y-1}; y > 0               && !visited(p1)) n_list.push_back(p1);
        if(const auto p1 = point_t{x+1, y  }; x < size - 1 && !visited(p1)) n_list.push_back(p1);
        if(const auto p1 = point_t{x  , y+1}; y < size - 1 && !visited(p1)) n_list.push_back(p1);
        return n_list;
    };

    const auto start_point = point_t{0,0};
    const auto end_point = point_t{size - 1, size - 1};
    dist(start_point) = 0;

    for(auto curr_point = start_point; 
             curr_point != end_point; 
             visited(curr_point) = true, curr_point = get_best_unvisited()) {
        for(const auto& np : get_neighs_not_visited(curr_point)) {
            const auto tentative_dist = dist(curr_point) + square_cost(np);
            if(tentative_dist < dist(np)) {
                update_unvisited(np, tentative_dist);
            }
        }
    }

    std::cout << dist(end_point) << std::endl;
}

int main() {
    execute(1);
    execute(5);
    return 0;
}