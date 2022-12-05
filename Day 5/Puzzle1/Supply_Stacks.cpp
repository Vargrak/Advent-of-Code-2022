#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;


//     [C]         [Q]         [V]    
//     [D]         [D] [S]     [M] [Z]
//     [G]     [P] [W] [M]     [C] [G]
//     [F]     [Z] [C] [D] [P] [S] [W]
// [P] [L]     [C] [V] [W] [W] [H] [L]
// [G] [B] [V] [R] [L] [N] [G] [P] [F]
// [R] [T] [S] [S] [S] [T] [D] [L] [P]
// [N] [J] [M] [L] [P] [C] [H] [Z] [R]
//  1   2   3   4   5   6   7   8   9 

int main()
{
    std::ifstream input_file;
    input_file.open("input.txt", std::ios::in);

    std::vector<vector<std::string>> stacks(9);
    
    for(std::string line; getline(input_file, line);)
    {
        int stack_i = 0;
        std::vector<std::string> split_line;
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);

        if (split_line[0] != "move")
        {
            for(size_t i = 0; i < line.size(); i += 4)
            {
                std::string box = line.substr(i, 4);
                if (box.find_first_not_of(" \n\r123456789") != std::string::npos)
                {
                    stacks[stack_i].insert(stacks[stack_i].begin(), box);
                }
                stack_i++;
            }
        }
        else if (split_line[0] == "move")
        {
            int total_to_move = stoi(split_line[1]);
            int move_from = stoi(split_line[3])-1;
            int move_to = stoi(split_line[5])-1;

            for(int boxes_moved = 0; boxes_moved < total_to_move; boxes_moved++)
            {
                stacks[move_to].push_back(stacks[move_from].back());
                stacks[move_from].pop_back();
            }
        }
    }
        for(int index = 0; index < stacks.size(); index++)
        {
            cout << "Stack: " << index << " Top Letter: " << stacks[index].back() << std::endl;
        }
}