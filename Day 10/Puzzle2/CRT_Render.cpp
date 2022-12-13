#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

void draw_sprite(int &v_register, std::string &render_frame)
{
    render_frame = std::string("........................................");

    if (v_register < 0)
    {
        int after_zero = 3 + v_register;
        render_frame.replace(0, after_zero, std::string(after_zero, '#'));
    }
    else render_frame.replace(v_register, 3, std::string(3, '#'));
}

void populate_instructions(std::ifstream &input, std::vector<std::pair<std::string, int>> &instructions)
{
    for(std::string line; getline(input, line);)
    {
        std::vector<std::string> split_line;
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);

        if (split_line[0] == "noop") instructions.push_back({split_line[0], 0});

        else instructions.push_back({split_line[0], stoi(split_line[1])});
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

    std::string render_frame("###.....................................");
    std::string raster_line;
    std::vector<std::string> image;

    populate_instructions(input_text, instructions);
    
    auto instruction = instructions.begin();
    int busy = 0;

    while (instruction != instructions.end())
    {
        raster_line.append(render_frame.substr(((clock_cycle%40) + 1), 1));
        clock_cycle++;
   
        //Every 40 cycles the raster needs to be pushed to the image and cleared.
        //So the next image can be rendered on the raster.
        if(clock_cycle % 40 == 0)
        {
            image.push_back(raster_line);
            raster_line.clear();
        }

        //Parse instructions
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
                draw_sprite(v_register, render_frame);
                instruction = next(instruction);
            }
        }
    }

    for(auto raster : image)
    {
        std::cout << raster << std::endl;
    }

    return 0;
}