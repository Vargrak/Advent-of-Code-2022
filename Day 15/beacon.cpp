#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "file_load.h"
#include "map.h"

//Load input / Check
//Create Map
//Populate Map with Sensors and Beacons
//Fill in sections covered by sensors
//Read map across y axis 2000000 and return how many positions cannot contain a beacon

int main()
{
    Map map = Map(load_file("input.txt"));

    
    std::cout << "Part 1: " << map.get_positions_without_beacons_along_y(2000000) << std::endl;
    long long int ans_2 = map.tuning_frequency(0, 4000000);




    std::cout << "Part 2: " << std::setprecision(30) << ans_2 << std::endl;

    return 0;
}