#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <bits/stdc++.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

class Monkey
{
    public:
    Monkey *true_receiver;
    Monkey *false_receiver;
    std::vector<unsigned long long int> items;
    std::string operation;
    std::string operator_;
    unsigned long long int times_inspected_item;
    int modulo;
    int monkey_id;
    int receiving_monkey_true;
    int receiving_monkey_false;
    int test_receiver;

    Monkey(int monkey_number)
    {
        this->times_inspected_item = 0;
        this->monkey_id = monkey_number;
    }

    void add_worry(int item)
    {
        if (this->operation == "old")
        {
            if (this->operator_ == "+") this->items[item] += this->items[item];
            else this->items[item] *= this->items[item];

        }
        else
        {
            if (this->operator_ == "+") this->items[item] += stoi(this->operation);
            else this->items[item] *= stoi(this->operation);
        }

        this->times_inspected_item++;
    }

    void relieve_worry(int item)
    {
        //Modulo instead of divide
        this->items[item] %= (this->modulo);
    }

    void pass_item(int item)
    {
        //If item value is divisible by the test, send to true receiver
        if (this->items[item] % this->test_receiver == 0)
        {
            this->true_receiver->items.push_back(this->items[item]);
        }
        else
        {
            this->false_receiver->items.push_back(this->items[item]);
        }
    }

    void play_turn()
    {
        //Pass item might cause issues with the iterator, for future reference
        for (int index = 0; index < this->items.size(); index++)
        {
            add_worry(index);
            relieve_worry(index);
            pass_item(index);
        }

        this->items.clear();
    }
};

//Separated from main for readability. 
//Gets input file, parses the lines for information about a monkey and adds them all ot a vector.
std::vector<Monkey *> get_monkies()
{
    //SET INPUT TEXT HERE    VVVVVVVVVVVVVVVV
    std::ifstream input_text("input.txt", std::ios::in);

    Monkey *monkey_construct;
    std::vector<Monkey *> Monkies;

    for (std::string line; getline(input_text, line);)
    {
        std::vector<std::string> split_line;
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);

        if (!line.empty())
        {
            if (split_line[0] == "Monkey") 
            {
                Monkies.push_back(new Monkey(stoi(split_line[1])));
                monkey_construct = Monkies[stoi(split_line[1])];
            }
            else if (split_line[1] == "Starting")
            {
                for (auto string : split_line)
                {
                    try
                    {
                        monkey_construct->items.push_back(stoi(string));
                    }
                    catch(...)
                    {
                        continue;
                    }
                }
            }

            //Gets whether Monkey multiplies or adds to worry level and if it does so by a fixed value or the item's own value.
            else if (split_line[1] == "Operation:") 
            { 
                monkey_construct->operator_ = split_line[5]; //+ or *
                monkey_construct->operation = split_line.back(); //Item's value or fixed number
            }
            else if (split_line[1] == "Test:") monkey_construct->test_receiver = stoi(split_line.back());
            else if (split_line[2] == "true:") monkey_construct->receiving_monkey_true = stoi(split_line.back());
            else if (split_line[2] == "false:") monkey_construct->receiving_monkey_false = stoi(split_line.back());
        }
    }

    return Monkies;
}

int main()
{
    std::vector<Monkey *> Monkies = get_monkies();
  
    std::vector<int> mods;
    for (auto monkey : Monkies)
    {
        mods.push_back(monkey->test_receiver);
    }

    //Need to set the receivers for each monkey
    for (auto monkey : Monkies)
    {
        monkey->modulo = std::accumulate(mods.begin(), mods.end(), 1, std::multiplies<int>());
        monkey->true_receiver = Monkies[monkey->receiving_monkey_true];
        monkey->false_receiver = Monkies[monkey->receiving_monkey_false];
    }

    //Play 20 turns
    for (int round_number = 1; round_number < 10001; round_number++)
    {
        for (auto monkey : Monkies)
        {   
            monkey->play_turn();
        }
    }

    //Vector for sorting and retrieving the amount of times the monkies inspected an item during play.
    std::vector<unsigned long long int> inspected_x_times;
    for (auto monkey : Monkies)
    {
        inspected_x_times.push_back(monkey->times_inspected_item);
    }

    sort(inspected_x_times.begin(), inspected_x_times.end(), std::greater<int>());

    //Level of Monkey Business is the two highest # of times monkies inspected items multiplied. 
    std::cout << inspected_x_times[0] * inspected_x_times[1] << std::endl;

    return 0;
}