#include <algorithm>
#include <iostream>
#include <math.h>
#include <set>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

class Map
{
    public:
        Map(std::vector<std::string> input)
        {
            set_map_size(input);


            //Find ranges covered by the sensors. Each y is a sensor and every x is a list of covered tiles
            for (int i = 0; i < this->sensors_.size(); i++)
            {
                auto sensor = this->sensors_[i];
                auto beacon = this->beacons_[i];

                int range = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second);

                //Expand range of map according to sensors range size
                if (sensor.first + range > this->x_size_)
                {
                    this->x_size_ = sensor.first + range;
                }
                if (sensor.second + range > this->y_size_)
                {
                    this->y_size_ = sensor.second + range;
                }
                if (sensor.first - range < this->x_start_)
                {
                    this->x_start_ = sensor.first - range;
                }
                if (sensor.second - range < this->y_start_)
                {
                    this->y_start_ = sensor.second - range;
                }
            }
        }

        //Members end with '_' to avoid confusion with other variables, similarly named
        int x_start_ = 0;
        int y_start_ = 0;
        int x_size_ = 0;
        int y_size_ = 0;
        int covered_tiles_ = -1;
        std::vector<std::pair<int, int>> beacons_;
        std::vector<std::pair<int, int>> sensors_;

        // For every x along y, check if the range of the sensor to the beacon is greater than or equal to the range of the sensor to the tile
        //Count is incremented if true
        int get_positions_in_range_along_y(int y)
        {
            for (int x = this->x_start_; x <= x_size_; x++)
            {
                 for (int i = 0; i < this->sensors_.size(); i++)
                 {
                        auto sensor = this->sensors_[i];
                        auto beacon = this->beacons_[i];
    
                        if (std::abs(sensor.first - x) + std::abs(sensor.second - y) <= std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second))
                        {
                            this->covered_tiles_++;
                            break;
                        }
                 }
            }

            return this->covered_tiles_;
        }

        //For every sensor, check if the range of the sensor to the beacon is greater than or equal to the range of the sensor to the tile
        bool in_range_of_all_sensors(int x, int y)
        {
            for (int i = 0; i < this->sensors_.size(); i++)
            {
                auto sensor = this->sensors_[i];
                auto beacon = this->beacons_[i];

                if (std::abs(sensor.first - x) + std::abs(sensor.second - y) <= std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second))
                {
                    return true;
                }
            }
            
            return false;
        }

        long long int tuning_frequency(int boundary_bottom, int boundary_top)
        {
            //For every sensor
            for (int i = 0; i < this->sensors_.size(); i++)
            {
                auto sensor = this->sensors_[i];
                auto beacon = this->beacons_[i];

                int radius = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second) + 1;

                //Scans the the tiles just outside of the perimeter.
                //Scans a diagonal line of the diamond at a time.
                //If the tile is not in range of any sensor and it is also within the bounds, the tuning frequency is returned
                //Must cast x and y to long long int to prevent overflow

                //Left to Bottom; x++ y++
                int start_x = sensor.first - radius;
                int start_y = sensor.second;
                for (int x = start_x; x <= sensor.first; x++)
                {
                    int y = start_y + (start_x - x);
                    if ( x >= boundary_bottom && x <= boundary_top && y >= boundary_bottom && y <= boundary_top && !in_range_of_all_sensors(x, y))
                    {
                        return (long long int)x * 4000000 + (long long int)y;
                    }

                }

                //Bottom to Right; x++ y--
                start_x = sensor.first;
                start_y = sensor.second + radius;
                for (int x = start_x; x <= sensor.first + radius; x++)
                {
                    int y = start_y - (x - start_x);
                    if (x >= boundary_bottom && x <= boundary_top && y >= boundary_bottom && y <= boundary_top && !in_range_of_all_sensors(x, y))
                    {
                        return (long long int)x * 4000000 + (long long int)y;
                    }
                }

                //Right to Top; x-- y--
                start_x = sensor.first + radius;
                start_y = sensor.second;
                for (int x = start_x; x >= sensor.first; x--)
                {
                    int y = start_y - (start_x - x);
                    if (x >= boundary_bottom && x <= boundary_top && y >= boundary_bottom && y <= boundary_top && !in_range_of_all_sensors(x, y))
                    {
                        return (long long int)x * 4000000 + (long long int)y;
                    }
                }

                //Top to Left; x-- y++
                start_x = sensor.first;
                start_y = sensor.second - radius;
                for (int x = start_x; x >= sensor.first - radius; x--)
                {
                    int y = start_y + (x - start_x);
                    if (x >= boundary_bottom && x <= boundary_top && y >= boundary_bottom && y <= boundary_top && !in_range_of_all_sensors(x, y))
                    {
                        return (long long int)x * 4000000 + (long long int)y;
                    }
                }
            }

            return 0;
        }


        //Get x and y of beacon from the half of the input string line that contains the beacon
        std::vector<std::pair<int, int>> get_beacons(std::vector<std::string> input)
        {
            std::cout << "Getting beacons... " << std::endl;
            for (auto line : input)
            {
                //Split line breaks into the section containing x and y
                std::vector<std::string> split_line;
                std::vector<std::string> vars_1;
                std::vector<std::string> vars_2;

                //vars 1 and 2 are the x and y of the sensor, identified by the number coming after the '='
                boost::split(split_line, line, boost::is_any_of(","), boost::token_compress_on);
                boost::split(vars_1, split_line[0], boost::is_any_of("="));
                boost::split(vars_2, split_line[1], boost::is_any_of("="));

                this->beacons_.push_back(std::make_pair(std::stoi(vars_1[1]), std::stoi(vars_2[1])));
            }

            return this->beacons_;
        }

        //Get x and y of sensor from the half of the input string line that contains the sensor
        std::vector<std::pair<int, int>> get_sensors(std::vector<std::string> input)
        {
            std::cout << "Getting sensors... " << std::endl;
            for (auto line : input)
            {
                //Split line breaks into the section containing x and y
                std::vector<std::string> split_line;
                std::vector<std::string> vars_1;
                std::vector<std::string> vars_2;

                //vars 1 and 2 are the x and y of the sensor, identified by the number coming after the '='
                boost::split(split_line, line, boost::is_any_of(","), boost::token_compress_on);
                boost::split(vars_1, split_line[0], boost::is_any_of("="));
                boost::split(vars_2, split_line[1], boost::is_any_of("="));

                this->sensors_.push_back(std::make_pair(std::stoi(vars_1[1]), std::stoi(vars_2[1])));
            }

            return this->sensors_;
        }

        void set_map_size(std::vector<std::string> input)
        {
            std::cout << "Setting initial map size... " << std::endl;
            std::pair<std::vector<std::string>, std::vector<std::string>> split_string = split_input(input);

            //Both beacons and sensors are stored as pairs of x and y
            //Their indices in the vector correspond to the same sensor/beacon pair
            this->sensors_ = get_sensors(split_string.first);
            this->beacons_ = get_beacons(split_string.second);
            int x_start = 0;
            int y_start = 0;
            int x_size = 0;
            int y_size = 0;

            //Sets initial map size to the largest x and y values of the beacon and sensors
            for (auto sensor : this->sensors_)
            {
                if (sensor.first > x_size)
                {
                    x_size = sensor.first;
                }
                else if (sensor.first < x_start)
                {
                    x_start = sensor.first;
                }
                if (sensor.second > y_size)
                {
                    y_size = sensor.second;
                }
                else if (sensor.second < y_start)
                {
                    y_start = sensor.second;
                }
            }

            for (auto beacon : this->beacons_)
            {
                if (beacon.first > x_size)
                {
                    x_size = beacon.first;
                }
                else if (beacon.first < x_start)
                {
                    x_start = beacon.first;
                }
                if (beacon.second > y_size)
                {
                    y_size = beacon.second;
                }
                else if (beacon.second < y_start)
                {
                    y_start = beacon.second;
                }
            }

            this->x_start_ = x_start;
            this->y_start_ = y_start;
            this->x_size_ = x_size;
            this->y_size_ = y_size;
        }

        //Take the input and split the strings between the sensor and beacon sections
        std::pair<std::vector<std::string>, std::vector<std::string>> split_input(std::vector<std::string> input)
        {
            std::vector<std::string> beacons;
            std::vector<std::string> sensors;
            for (auto line : input)
            {
                std::vector<std::string> split_line;
                boost::split(split_line, line, boost::is_any_of(":"));
                beacons.push_back(split_line[0]);
                sensors.push_back(split_line[1]);                
            }
            return std::make_pair(beacons, sensors);
        }
};
