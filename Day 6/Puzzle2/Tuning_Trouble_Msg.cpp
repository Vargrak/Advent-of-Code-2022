#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

bool vec_has_duplicate_char(std::vector<char> buffer)
{
    //For all elements
    for(int i = 0; i < buffer.size(); i++)
    {
        //Comparison char
        char compare = buffer[i];

        //For every element after the comparison element
        for(int j = i+1; j < buffer.size(); j++)
        {
            //If equal, true
            if (compare == buffer[j])
            {
                return true;
            }
        }
    }

    //Fall through for loop to false
    return false;
}



int main()
{
    ifstream input_text;
    input_text.open("input.txt");

    int start_of_packet_char = 0;
    std::vector<char> buffer;
    for(std::string line; getline(input_text, line);)
    {
        //For every char in line
        for(int i = 0; i < line.length(); i++)
        {
            //Simply adjusted buffer size for part 2
            if (buffer.size() == 14)
            {   
                //Check for duplicate
                if (!vec_has_duplicate_char(buffer))
                {
                    //Cout and quit packet start
                    cout << "Start of packet found at char: " << start_of_packet_char << std::endl;
                    return 0;
                }
                else
                {
                    //Else clear first element
                    buffer.erase(buffer.begin());
                }
            }
            //push new char and increment counter
            buffer.push_back(line[i]);
            start_of_packet_char++;
        }
    }
}