#include <fstream>
#include <string>
#include <vector>

//Takes an input file and reads it into a vector of strings for repeated reading.
std::vector<std::string> load_file(std::string file_select)
{
    std::ifstream input_file(file_select);
    std::vector<std::string> input;

    for (std::string line; std::getline(input_file, line);)
    {
        input.push_back(line);
    }

    return input;
}