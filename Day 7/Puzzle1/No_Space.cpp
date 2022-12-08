#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <filesystem>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/variant.hpp>

using namespace std;

/* Parsing
$ token -> cd or ls command
if cd taken next token as path append then normalize
    if ls take the current directory and add the rest as contents ->
        if dir, file name is added to directory as directory
        if else it is added as file.
*/


bool is_command(std::string token)
{
    if (token == "$")
        return true;
    return false;
}

bool dir_flag_enabled(int flag)
{
    if (flag == 1)
        return true;
    return false;
}


int main()
{
    ifstream input_text;
    input_text.open("input.txt");

    std::map<std::string, int> dir;
    std::vector<std::string> split_line;
    std::filesystem::path directory_tree;
    std::filesystem::path curr_directory;

    int dir_read_flag = 0;
    for(std::string line; getline(input_text, line);)
    {
        boost::split(split_line, line, boost::is_any_of(" "), boost::token_compress_on);

        if (is_command(split_line[0]))
        {
            dir_read_flag = 0;
            if (split_line[1] == "cd")
            {
                curr_directory /= split_line[2];
                curr_directory = curr_directory.lexically_normal();
                //cout << curr_directory << std::endl;
            }
            else if (split_line[1] == "ls")
            {
                dir_read_flag = 1;
            }
        }
        else if (dir_flag_enabled(dir_read_flag))
        {
            std::filesystem::path tmp = curr_directory;
            if (split_line[0] == "dir")
            {
                dir.emplace((curr_directory /= split_line[1]).string(), 0)
            }
            else if (std::all_of(split_line[0].begin(), split_line[0].end(), ::isdigit))
            {
                dir.emplace(tmp.string(), split_line[0]);
            }
        }
        
    }


    std::string curr_key;
    for (auto itr = dir.begin(); itr != dir.end(); itr++)
    {
        if (curr_key == itr->first)
            continue;

        else
            curr_key = itr->first;

        const auto result = dir.equal_range(itr->first);
        std::string parse_key = itr->first;

        cout << itr->first << ": ";
        for (auto it = result.first; it != result.second; it++)
        {
            cout << it->second << " ";
        }
        cout << std::endl;
    
    }

}