#include <vector>
#include <stack>
#include <string>
#include <map>
#include <numeric>
#include <iostream>
#include <fstream>

//370407
//3249889609
const std::map<char, int> error_lut {
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137},
};
const std::map<char, int> complete_lut {
    {'(', 1},
    {'[', 2},
    {'{', 3},
    {'<', 4},
};
const std::map<char, char> close_lut {
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'},
};

int main()
{
    const auto input = [](){
        std::vector<std::string> input;
        if(std::ifstream input_file("input"); input_file.is_open()) {
            /* O(n) */
            for(std::string line_str; std::getline(input_file, line_str, '\n');) {
                input.push_back(line_str);
            }
        }
        return input;
    }();
    const auto [corrupt_score, autocomplete_scores] = 
        std::accumulate(input.begin(), input.end(), 
            std::pair{0ULL, std::vector<unsigned long long>{}},
            [](auto acc, const auto line) {
                auto& [c_score, a_scores] = acc;
                std::stack<char> chunk_op;
                for(const char ch : line) {
                    switch(ch) {
                    case '(': case '[': case '{': case '<':
                        chunk_op.push(ch);
                        break;
                    default:
                        if(close_lut.at(chunk_op.top()) == ch) {
                            chunk_op.pop();
                            break;
                        } else {
                            return std::pair{c_score + error_lut.at(ch), std::move(a_scores)};
                        }
                    }
                }
                auto a_score = 0ULL;
                for(;!chunk_op.empty(); chunk_op.pop()) {
                    a_score = a_score*5 + complete_lut.at(chunk_op.top());
                }

                a_scores.insert(std::upper_bound(a_scores.begin(), a_scores.end(), a_score), a_score);
                return std::pair{c_score, std::move(a_scores)};
            }
        );

    std::cout << corrupt_score << std::endl;
    std::cout << autocomplete_scores.at(autocomplete_scores.size() / 2) << std::endl;

    return 0;
}