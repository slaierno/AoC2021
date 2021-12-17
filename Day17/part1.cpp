#include <algorithm>
#include <set>
#include <string>
#include <execution>
#include <set>
#include <iostream>
#include <utils.hpp>

using point_t = std::pair<int, int>;
using vel_t = std::pair<int, int>;

int main() {
    const auto target_range = std::pair{std::pair{20,30}, std::pair{-10,-5}};

    const auto in_range = [](const int val, const auto range) {
        if(val < range.first) return -1;
        if(val > range.second) return  1;
        return 0;
    };
    // const auto find_viable_vel = [in_range](const auto target_range, const auto step_vel){
    //     const auto [target_min, target_max] = target_range;
    //     std::vector<int> target_vels;
    //     for(int vel_0 = target_max; vel_0 > 0; vel_0-=(target_max > 0 ? 1 : -1)) {
    //         int vel = vel_0;
    //         int pos = 0;
    //         bool vel_found = false;
    //         for(int step = 0; step <= vel_0 || pos >= target_min; step++) {
    //             int old_pos = pos;
    //             pos += vel;
    //             vel = step_vel(vel);
    //             if(in_range(pos, target_range) == 0) {
    //                 vel_found = true;
    //                 break;
    //             } else if (in_range(pos, target_range) != in_range(old_pos, target_range)) break;
    //         }
    //         if(vel_found) {
    //             target_vels.push_back(vel_0);
    //             continue;
    //         }
    //     }
    //     return target_vels;
    // };

    // for(const auto target_vel : 
    //     find_viable_vel(target_range.first, 
    //     [](const auto vel_x) {return std::max(0, vel_x - 1);})
    // ) {
    //     std::cout << target_vel << std::endl;
    // }
    // for(const auto target_vel : 
    //     find_viable_vel(target_range.second, 
    //     [](const auto vel_y) {return vel_y - 1;})
    // ) {
    //     std::cout << target_vel << std::endl;
    // }

    const auto a = [](const auto s, const auto n) { return s*((2*n + 1) - s) / 2;};
    std::set<int> viable_steps;
    const auto tx_max = target_range.first.second;
    for(int vx0 = 1; vx0 <= tx_max; vx0++) {
        for(int step = 1; step <= vx0; step++) {
            const auto v_step = a(step, vx0);
            if(auto range_res = in_range(v_step, target_range.first); range_res == 0) {
                viable_steps.emplace(step);
            } else if (range_res == 1) {
                break;
            }
        }
    }
    // const auto ty_max = target_range.second.second;
    auto max_y_pos = 0;
    // auto max_step = 0;
    for(int vy0 = 0; true; vy0++) {
        const auto last_possible_y_pos = a(*viable_steps.rbegin(), vy0);
        if(in_range(last_possible_y_pos, target_range.second) == 1) {
            break;
        }
        for(const auto step : viable_steps) {
            const auto y_pos = a(step, vy0);
            const auto range_res = in_range(y_pos, target_range.second);
            if(range_res == 0) {
                const auto highest_y_pos = a(vy0, vy0); 
                if (highest_y_pos > max_y_pos) {
                    max_y_pos = highest_y_pos;
                }
                break;
            } else if (range_res == -1) {
                break;
            }
        }
    }

    std::cout << max_y_pos << std::endl;
    return 0;
}