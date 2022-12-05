#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

int main()
{
    std::ifstream input_file;
    input_file.open("input.txt", std::ios::in);

    int fully_contains_count = 0;
    for(std::string line; getline(input_file, line);)
    {
        std::vector<std::string> pair;
        std::vector<std::string> first_pair_range;
        std::vector<std::string> second_pair_range;

        boost::split(pair, line, boost::is_any_of(","), boost::token_compress_on);
        boost::split(first_pair_range, pair[0], boost::is_any_of("-"), boost::token_compress_on);
        boost::split(second_pair_range, pair[1], boost::is_any_of("-"), boost::token_compress_on);

        if (stoi(first_pair_range[0]) <= stoi(second_pair_range[0]) && stoi(first_pair_range[1]) >= stoi(second_pair_range[1]) 
        || stoi(second_pair_range[0]) <= stoi(first_pair_range[0]) && stoi(second_pair_range[1]) >= stoi(first_pair_range[1]))
        {
            fully_contains_count++;
        }
    }

    cout << fully_contains_count << std::endl;
}