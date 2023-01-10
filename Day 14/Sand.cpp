#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "Map.h"

std::vector<std::string> store_input(std::string file_select)
{
    std::ifstream input_file(file_select);
    std::vector<std::string> input;

    for (std::string line; std::getline(input_file, line);)
    {
        input.push_back(line);
    }

    return input;
}

std::vector<std::vector<std::pair<int,int>>> convert_input_to_vectors(std::vector<std::string> input)
{
    std::vector<std::vector<std::pair<int,int>>> vectors;

    for (auto line : input)
    {
        std::vector<std::string> vector_strings;
        boost::split(vector_strings, line, boost::is_any_of("-> "), boost::token_compress_on);
    
        std::vector<std::pair<int,int>> vector;

        for (auto vector_string : vector_strings)
        {
            std::vector<std::string> vector_components;
            boost::split(vector_components, vector_string, boost::is_any_of(","), boost::token_compress_on);

            vector.push_back(std::make_pair(std::stoi(vector_components[0]), std::stoi(vector_components[1])));
        }

        vectors.push_back(vector);
    }

    return vectors;
}

int main()
{
    std::vector<std::vector<std::pair<int,int>>> vectors = convert_input_to_vectors(store_input("input.txt"));

    Map map_void(vectors, false);
    Map map_floor(vectors, true);

    map_void.convert_vectors_to_tiles(vectors);
    map_floor.convert_vectors_to_tiles(vectors);

    std::cout << "Sand counter no floor " << map_void.sand_simulate() << std::endl;
    std::cout << "Sand counter floor " << map_floor.sand_simulate() << std::endl;
    
    return 0;
}

