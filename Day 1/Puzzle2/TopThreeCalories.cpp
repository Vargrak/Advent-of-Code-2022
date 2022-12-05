#include <fstream>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;

//Run through lines once for line count
int line_counter()
{
    fstream text_file;
    text_file.open("input.txt");

    std::string line;
    std::size_t lines_count = 0;

    //Count every non-empty line
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
    //Get file
    fstream text_file;
    text_file.open("input.txt");

    std::string line;
    std::size_t lines_count = line_counter();

    int index = 0;
    int calorie_total[lines_count];

    //Set calorie_total to all zeros
    memset(calorie_total, 0, sizeof(calorie_total));

    while (std::getline(text_file, line))
    {
        //cout << "LINE: " << line << "\n";
        //If only "\n" increment index
        if (line.empty())
        {
            //cout << "SUM: " << calorie_total[index] << "\n";
            index++;
        }
        //Else cast to int and add to index
        else
        {
            calorie_total[index] += stoi(line);
        }
    }
    //cout << "SUM: " << calorie_total[index] << "\n";

    //Sort descending
    sort(calorie_total, calorie_total + lines_count, greater<int>());

    //Print top three out
    std::cout << calorie_total[0] + calorie_total[1] + calorie_total[2] << std::endl;
    return 0;
}