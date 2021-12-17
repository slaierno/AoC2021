#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

struct Instruction {
    enum {
        FW,
        UP,
        DN
    } direction;
    unsigned val;
    Instruction(std::string str) {
        auto sep = str.find(' ');
        if(const auto sstr = str.substr(0, sep); sstr == "forward") {
            direction = FW;
        } else if (sstr == "up") {
            direction = UP;
        } else if (sstr == "down") {
            direction = DN;
        }
        val = std::stoull(str.substr(sep+1));
    }
};

// 1762050
// 1855892637
int main()
{
    std::vector<Instruction> input;
    if(std::ifstream input_file("input"); input_file.is_open()) {
        /* O(n) */
        for(std::string inst; std::getline(input_file, inst, '\n');) {
            input.emplace_back(inst);
        }
    }
    {
        int hor = 0, depth = 0;
        for(const auto& inst : input) {
            switch(inst.direction) {
                case Instruction::FW: hor+=inst.val; break;
                case Instruction::DN: depth+=inst.val; break;
                case Instruction::UP: depth-=inst.val; break;
            }
        }
        std::cout << hor * depth << std::endl;
    }
    {
        int aim = 0, hor = 0, depth = 0;
        for(const auto& inst : input) {
            switch(inst.direction) {
                case Instruction::FW: hor+=inst.val; depth+=inst.val*aim; break;
                case Instruction::DN: aim+=inst.val; break;
                case Instruction::UP: aim-=inst.val; break;
            }
        }
        std::cout << hor * depth << std::endl;
    }
    return 0;
}