#include <set>
#include <map>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>

//7381
//3019
int main()
{
    // constexpr auto target_area = std::pair{std::pair{20, 30}, std::pair{-10, -5}};
    constexpr auto target_area = std::pair{std::pair{185, 221}, std::pair{-122, -74}};
    constexpr auto tx_min = target_area.first.first;
    constexpr auto tx_max = target_area.first.second;
    constexpr auto ty_min = target_area.second.first;
    constexpr auto ty_max = target_area.second.second;

    static_assert(ty_min < 0 && ty_max < 0);

    constexpr auto a = [](const auto step, const auto vel) { return step * (2*vel + 1 - step) / 2; };

    // Viable ranges are vx_rng = [z; tx_max] and vy_rng = [ty_min, -ty_min]; where z is the biggest solution of a(n,n) = tx_max, i.e. floor(sqrt(2*tx_min+.25)-.5)

    std::map<int, std::set<int>> viable_steps;
    for(int vx0 = std::sqrt(.25+2*tx_min) - .5; vx0 <= tx_max; vx0++) {
        for(int step = vx0; step >= 0; step--) {
            if(const int x_pos = a(step, vx0); x_pos < tx_min) break;
            else if(x_pos <= tx_max) {
                viable_steps[vx0].insert(step);
                if(step == vx0)
                    viable_steps[vx0].emplace_hint(viable_steps[vx0].end(), std::numeric_limits<int>::max());
            }
        }
    }
    int best_y_pos = 0;
    int success_cnt = 0;
    for(int vy0 = -ty_min; vy0 >= ty_min; vy0--) {
        for(const auto& entry : viable_steps) {
            bool go_towards_infty = false;
            const auto& steps = entry.second;
            auto step_it = steps.begin();
            int step = *step_it;
            while(step_it != steps.end()) {
                const int y_pos = a(step, vy0);
                if(y_pos < ty_min) break;
                if(ty_min <= y_pos && y_pos <= ty_max) {
                    success_cnt++;
                    if(vy0 > 0) best_y_pos = std::max(a(vy0, vy0), best_y_pos);
                    break;
                }
                if(go_towards_infty) step++;
                else {
                    const auto prev_step = step;
                    step_it = std::next(step_it);
                    step = *step_it;
                    if(step == std::numeric_limits<int>::max()) {
                        step = prev_step;
                        go_towards_infty = true;
                    }
                }
            }
        }
    }
    
    std::cout << best_y_pos << std::endl;
    std::cout << success_cnt << std::endl;
    return 0;
}