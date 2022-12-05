#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int get_letter_priority(char letter)
{
    std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return (letters.find(letter)+1);
}

int compare_matching_elements(vector<string> group)
{
    for(std::string::iterator first_itr = group[0].begin(); first_itr != group[0].end(); first_itr++)
    {
        for(std::string::iterator second_itr = group[1].begin(); second_itr != group[1].end(); second_itr++)
        {
            if (*first_itr == *second_itr)
            {
                for (std::string::iterator third_itr = group[2].begin(); third_itr != group[2].end(); third_itr++)
                {
                    if (*first_itr == *third_itr)
                    {
                        return get_letter_priority(*first_itr);
                    }
                }
            }

        }
    }
    
    return 0;
}

int main()
{
    ifstream input_file;
    input_file.open("input.txt", std::ios::in);

    std::string line;
    std::vector<string> group;

    int priority_total = 0;
    while(getline(input_file, line))
    {
        group.push_back(line);
        if (group.size() == 3)
        {
            priority_total += compare_matching_elements(group);
            group.clear();
        }
    }

    cout << priority_total << std::endl;
}