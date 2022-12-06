#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

bool vec_has_duplicate_char(std::vector<char> buffer)
{
    for(int i = 0; i < buffer.size(); i++)
    {
        char compare = buffer[i];
        for(int j = i+1; j < buffer.size(); j++)
        {
            if (compare == buffer[j])
            {
                return true;
            }
        }
    }

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
        for(int i = 0; i < line.length(); i++)
        {
            if (buffer.size() == 4)
            {
                cout << "Current buffer: " << buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
                if (!vec_has_duplicate_char(buffer))
                {
                    cout << "Start of packet found at char: " << start_of_packet_char << std::endl;
                    return 0;
                }
                else
                {
                    buffer.erase(buffer.begin());
                }
            }
            buffer.push_back(line[i]);
            start_of_packet_char++;
        }
    }
}