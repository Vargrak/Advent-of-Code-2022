#include <fstream>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;

int line_counter()
{
    fstream text_file;
    text_file.open("input.txt");

    std::string line;
    std::size_t lines_count = 0;

    while (std::getline(text_file, line))
    {
        if (!line.empty())
        {
            lines_count++;
        }
    }

    return lines_count;
}

int main()
{
    fstream text_file;
    text_file.open("input.txt");

    std::string line;
    std::size_t lines_count = line_counter();

    int index = 0;
    int calorie_total[lines_count];
    memset(calorie_total, 0, sizeof(calorie_total));

    while (std::getline(text_file, line))
    {
        //cout << "LINE: " << line << "\n";
        if (line.empty())
        {
            //cout << "SUM: " << calorie_total[index] << "\n";
            index++;
        }
        else
        {
            calorie_total[index] += stoi(line);
        }
    }
    //cout << "SUM: " << calorie_total[index] << "\n";

    int n = sizeof(calorie_total) / sizeof(calorie_total[0]);
    sort(calorie_total, calorie_total + n, greater<int>());

    std::cout << calorie_total[0] << std::endl;
    return 0;
}