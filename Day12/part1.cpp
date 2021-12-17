#include <map>
#include <algorithm>
#include <list>
#include <string>
#include <execution>
#include <set>
#include <iostream>
#include <utils.hpp>

//3510
int explore_children(const auto& graph, const auto& start_node, const auto& small_cave_visited) {
    constexpr auto is_small_cave = [](const auto cave) {
        return std::all_of(std::begin(cave), std::end(cave), [](const auto ch){return std::islower(ch);});
    };
    return std::transform_reduce(std::execution::par_unseq, std::begin(start_node), std::end(start_node), 0, std::plus<>{}, [&](const auto& child){
        if(child == "start") return 0;
        if(child == "end") return 1;
        auto new_cave_list = small_cave_visited;
        if (is_small_cave(child)) {
            if(small_cave_visited.contains(child)) return 0;
            new_cave_list.insert(child);
        }
        return explore_children(graph, graph.at(child), new_cave_list);
    });
};

int main() {
    const auto graph = []() {
        std::map<std::string, std::list<std::string>> graph;
        const auto input = AoC::get_input("input.txt", '\n');
        for(const std::string& line_str : input) {
            const auto connection = AoC::split(line_str, '-');
            if (connection.size() > 2) throw;
            graph[connection[0]].push_back(connection[1]);
            graph[connection[1]].push_back(connection[0]);
        }
        return graph;
    }();

    const auto path_count = [&]() {
        std::set<std::string> small_cave_visited;
        return explore_children(graph, graph.at("start"), small_cave_visited);
    }();

    std::cout << path_count << std::endl;
    return 0;
}