#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

void populate_instructions(std::ifstream &input, std::vector<std::pair<std::string, int>> &instructions)
{
    for(std::string line; getline(input, line);)
    {
        std::vector<std::string> split_line;
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);

        if (split_line[0] == "noop")
        {
            instructions.push_back({split_line[0], 0});
        }
        else
        {
            instructions.push_back({split_line[0], stoi(split_line[1])});
        }
    }
}

int main()
{
    std::ifstream input_text("input.txt", std::ios::in);
    std::vector<std::pair<std::string, int>> instructions;
    int v_register = 1;
    int clock_cycle = 0;
    int cycle_check = 20;
    int signal_strength_sum = 0;

    populate_instructions(input_text, instructions);
    
    auto instruction = instructions.begin();
    int busy = 0;
    while (instruction != instructions.end())
    {
        clock_cycle++;

        if (clock_cycle % cycle_check == 0)
        {
            signal_strength_sum += (clock_cycle * v_register);
            if (cycle_check < 220) cycle_check += 40;
        }

        if (instruction->first == "addx" && busy == 0) busy = 1;
        else if (instruction->first == "noop")
        {
            instruction = next(instruction);
        }
        else if (busy > 0)
        {
            busy--;

            if (busy == 0)
            {
                v_register += instruction->second;
                instruction = next(instruction);
            }
        }
    }

    std::cout << "Signal Strength Sum: " << signal_strength_sum << std::endl;

    return 0;
}