#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <filesystem>

std::pair<int, int> move_knot(std::pair<int, int> head, std::pair<int, int> tail, std::set<std::pair<int, int>> &unique_positions, int tail_trail)
{
   while (abs(head.first - tail.first) > 1 || abs(head.second - tail.second) > 1)
    {
        
        if (head.first != tail.first && head.second != tail.second)
        {
            if (head.first > tail.first && head.second > tail.second)
            {
                tail.first++;
                tail.second++;
                unique_positions.insert(tail);
            }

            else if (head.first < tail.first && head.second < tail.second)
            {
                tail.first--;
                tail.second--;
                unique_positions.insert(tail);
            }

            else if (head.first > tail.first && head.second < tail.second)
            {
                tail.first++;
                tail.second--;
                unique_positions.insert(tail);
            }

            else if (head.first < tail.first && head.second > tail.second)
            {
                tail.first--;
                tail.second++;
                unique_positions.insert(tail);
            }
        }

        else if (head.first != tail.first && head.second == tail.second)
        {
            if (head.first > tail.first)
            {
                tail.first++;
                unique_positions.insert(tail);
            }

            else if (head.first < tail.first)
            {
                tail.first--;
                unique_positions.insert(tail);
            }
        }

        else if (head.first == tail.first && head.second != tail.second)
        {
            if (head.second > tail.second)
            {
                tail.second++;
                unique_positions.insert(tail);
            }

            else if (head.second < tail.second)
            {
                tail.second--;
                unique_positions.insert(tail);
            }
        }
    }

    return tail;
}

int unique_pos_visited(std::vector<std::pair<int,int>> commands, int tail_trail)
{
    std::vector<std::pair<int, int>> rope(tail_trail, std::make_pair(0,0));
    std::set<std::pair<int, int>> unique_positions = {std::make_pair(0,0)};
    std::set<std::pair<int, int>> unique_positions_end = {std::make_pair(0,0)};


    for(auto command : commands)
    {
        for(auto knot = rope.begin(); knot != rope.end(); knot++)
        {

            std::pair<int, int> head = *prev(knot);
            if (knot == rope.begin()) 
            {
                knot->first += command.first;
                knot->second += command.second;
            }
            else if (next(knot) == rope.end())
            {
                *knot = move_knot(head, *knot, unique_positions_end, tail_trail);
            }
            else
            {
                *knot = move_knot(head, *knot, unique_positions, tail_trail);
            }
            
        }
    }

    return unique_positions_end.size();
}

int main()
{
    std::ifstream input_text("input.txt");
    std::vector<std::pair<int,int>> commands;
    int unique_positions = 0;

    for(std::string line; std::getline(input_text, line);)
    {
        for(int i = 0; i < stoi(line.substr(1, line.length())); i++)
        {
            if (line[0] == 'U') commands.push_back(std::make_pair(0, 1));
            if (line[0] == 'D') commands.push_back(std::make_pair(0, -1));
            if (line[0] == 'L') commands.push_back(std::make_pair(-1, 0));
            if (line[0] == 'R') commands.push_back(std::make_pair(1, 0));
        }
        
    }

    unique_positions = unique_pos_visited(commands, 10);

    std::cout << unique_positions << std::endl;
}