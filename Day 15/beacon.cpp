#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "file_load.h"
#include "map.h"

int main()
{
    Map map = Map(load_file("input.txt"));

    std::cout << "Part 1: " << map.get_positions_in_range_along_y(2000000) << std::endl;

    std::cout << "Part 2: " << std::setprecision(30) << map.tuning_frequency(0, 4000000) << std::endl;

    return 0;
}