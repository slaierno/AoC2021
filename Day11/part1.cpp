#include <array>
#include <algorithm>
#include <numeric>
#include <string>
#include <iostream>
#include <mutex>
#include <execution>
#include <range/v3/all.hpp>
#include <utils.hpp>

constexpr int size = 10;
constexpr int steps = 100;
constexpr auto policy = std::execution::par_unseq;
namespace views = ranges::views;

//1642

using point = std::pair<int, int>;
class octopus {
    int m_energy;
    point m_pos;
    std::mutex m_mtx;
    bool m_flash_done {false};
public:
    octopus(auto energy, point pos) : m_energy(energy), m_pos(pos) {}
    octopus(auto energy, auto y, auto x) : octopus(energy, point(x, y)) {}
    octopus(const octopus& o) : octopus(o.m_energy, o.m_pos) {}
    constexpr void inc_unsafe() { ++m_energy; }
    auto inc() {
        std::lock_guard g(m_mtx);
        return inc_unsafe();
    }

    constexpr void set_zero_unsafe() { m_energy = 0; m_flash_done = false; }
    void set_zero() {
        std::lock_guard g(m_mtx);
        set_zero_unsafe();
    }

    auto get() const { return m_energy; }
    auto get_pos() const { return m_pos; }
    bool is_flashing() {
        std::lock_guard g(m_mtx);
        if (m_energy > 9 && !m_flash_done) {
            m_flash_done = true;
            return true;
        } else return false;
    }
};

auto get_neigh(const point& p) {
    const auto& [x, y] = p;
    return 
        views::cartesian_product(views::iota(x-1, x+2), views::iota(y-1, y+2)) | 
        views::filter([=](const auto p) {
            const auto [i, j] = p;
            return !(i == x && j == y) && i >= 0 && j >= 0 && i < size && j < size;
        });
}

class octo_map {
    std::vector<octopus> m_map;
    int m_flash_count {0};
    std::mutex m_mtx;

    void flash(octopus& o) {
        {
            std::lock_guard g(m_mtx);
            m_flash_count++;
        }
        for(const auto& [n_x, n_y] : get_neigh(o.get_pos())) {
            auto& neigh_o = m_map.at(n_x + n_y * size);
            neigh_o.inc();
            if(neigh_o.is_flashing()) {
                flash(neigh_o);
            }
        }
    }
public:
    octo_map(const auto& matrix) {
        if(matrix.size() != matrix.back().size() || matrix.size() != size) throw;
        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                m_map.emplace_back(matrix[i][j], i, j);
    }

    auto simulate_step() {
        std::for_each(policy, m_map.begin(), m_map.end(), [](auto& o){o.inc_unsafe();});
        std::for_each(policy, m_map.begin(), m_map.end(), [this](auto& o) {
            if(o.is_flashing()) {
                flash(o);
            }
        });
        std::for_each(policy, m_map.begin(), m_map.end(), [](auto& o) {
            if(o.get() > 9) {
                o.set_zero_unsafe();
            }
        });
    }
    void print() const {
        for(size_t i = 0; i < m_map.size(); i++) {
            std::cout << m_map[i].get();
            if((i + 1) % size == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    auto get_flash_count() const {
        return m_flash_count;
    }
};

int main() {
    const auto input = AoC::get_input("input.txt", '\n', [](const std::string& line_str) -> std::vector<int> {
        std::vector<int> line;
        std::transform(std::begin(line_str), std::end(line_str), std::back_inserter(line), [](const auto ch){return ch - '0';});
        return line;
    });

    octo_map omap(input);
    omap.print();

    for(int i = 0; i < steps; i++) {
        omap.simulate_step();
        omap.print();
    }
    std::cout << omap.get_flash_count() << std::endl;
    return 0;
}