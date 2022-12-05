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


int main()
{
    ifstream input_file;
    input_file.open("input.txt", std::ios::in);

    std::string line;
    std::vector<string> split_line;

    int priority_total = 0;
    while(getline(input_file, line))
    {
        int priority = 0;
        int length = line.length();
        split_line.push_back(line.substr(0, length/2));
        split_line.push_back(line.substr((length/2, length/2)));

        for(std::string::iterator first_itr = split_line[0].begin(); first_itr != split_line[0].end(); first_itr++)
        {
            for(std::string::iterator second_itr = split_line[1].begin(); second_itr != split_line[1].end(); second_itr++)
            {
                if (*first_itr == *second_itr)
                {
                   priority = get_letter_priority(*first_itr);
                }
            }
        }

        priority_total += priority;
        split_line.clear();
    }

    cout << priority_total << std::endl;
}