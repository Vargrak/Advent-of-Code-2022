#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <bits/stdc++.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

//Gets the file with weighted nodes and returns them as a vector of strings so it's 2d parseable
std::vector<std::string> get_map_vector()
{
    std::ifstream file("input.txt");
    std::vector<std::string> map_vector;

    for (std::string line; std::getline(file, line);)
    {
        map_vector.push_back(line);
    }

    return map_vector;
}




