#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <filesystem>

std::pair<int, int> move_head(std::string command, std::pair<int, int> pair)
{
    if (command[0] == 'U')
    {
        pair.second += stoi(command.substr(1, command.length()));
    }
    else if (command[0] == 'D')
    {
        pair.second -= stoi(command.substr(1, command.length()));
    }
    else if (command[0] == 'L')
    {
        pair.first -= stoi(command.substr(1, command.length()));
    }
    else if (command[0] == 'R')
    {
        pair.first += stoi(command.substr(1, command.length()));
    }
    return pair;
}

std::pair<int, int> move_tail(std::pair<int, int> head, std::pair<int, int> tail, std::set<std::pair<int, int>> &unique_positions, int tail_trail)
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


int unique_pos_visited(std::vector<std::string> commands, int tail_trail)
{
    std::pair<int, int> head = {0, 0};
    std::pair<int, int> tail = {0, 0};
    std::set<std::pair<int, int>> unique_positions;

    unique_positions.insert(tail);
    for (std::string command : commands)
    {
        head = move_head(command, head);
        tail = move_tail(head, tail, unique_positions, tail_trail);
    }

    return unique_positions.size();
}

int main()
{
    std::ifstream input_text("input.txt");
    std::vector<std::string> commands;
    int unique_positions = 0;

    for(std::string line; std::getline(input_text, line);)
    {
        commands.push_back(line);
    }

    unique_positions = unique_pos_visited(commands, 2);


    std::cout << unique_positions << std::endl;
}