#include <iostream>
#include <vector>
#include "Tile.h"

class Map
{
    public:
        Map(std::vector<std::vector<std::pair<int,int>>> set_vectors, bool has_floor)
        {
            //Sets the bounds of the map, specifically needed for x dimension as it does not start at zero
            for (auto vectors : set_vectors)
            {
                for (auto vector : vectors)
                {
                    if (vector.first < this->lower_width_bound)
                    {   
                        this->lower_width_bound = vector.first;
                    }
                    else if (vector.first > this->upper_width_bound)
                    {
                        this->upper_width_bound = vector.first;
                    }
                    if (vector.second < this->lower_height_bound)
                    {
                        this->lower_height_bound = vector.second;
                    }
                    else if (vector.second > this->upper_height_bound)
                    {
                        this->upper_height_bound = vector.second;
                    }
                }
            }

            if (has_floor)
            {
                this->lower_width_bound -= 100000;
                this->upper_width_bound += 100000;
                this->upper_height_bound += 2;
            }

            //Populates the new bounds with empty space tiles as '.'
            for (int y = this->lower_height_bound; y <= this->upper_height_bound; y++)
            {
                std::vector<Tile> row;
                for (int x = this->lower_width_bound; x <= this->upper_width_bound; x++)
                {
                    row.push_back(Tile(x, y, '.'));
                }
                this->tiles.push_back(row);
            }

            if (has_floor)
            {
                for (auto x = lower_width_bound; x <= upper_width_bound; x++)
                {
                    this->tiles[upper_height_bound][x - lower_width_bound].set_rock();
                }
            }

            this->y_size = this->upper_height_bound - this->lower_height_bound;
            this->x_size = this->upper_width_bound - this->lower_width_bound;
        }
        
        //Lower width bound set to 10000 as it could start above zero, height always starts at zero
        //2d vector is still zero indexed and only starts at the lowest vector's number
        //Subtracting the lower bound from the vector's number will give the correct index
        int lower_width_bound = 10000;
        int lower_height_bound = 0;
        int upper_width_bound = 0;
        int upper_height_bound = 0;
        int y_size = 0;
        int x_size = 0;
        std::vector<std::vector<Tile>> tiles;

        void add_tile(Tile tile)
        {
            this->tiles[tile.x - lower_width_bound][tile.y - lower_height_bound] = tile;
        }

        void render_map()
        {
            for (auto row : this->tiles)
            {
                for (auto tile : row)
                {
                    std::cout << tile.type;
                }
                std::cout << std::endl;
            }
        }

        //Change tile char
        void set_tile_type(int x, int y, char type)
        {
            this->tiles[y][x].type = type;
        }

    bool sand_can_move(int x, int y)
    {
        if (y + 1 > this->upper_height_bound || x - 1 < 0 || x + 1 > this->upper_width_bound - this->lower_width_bound)
        {
            return false;
        }

        char south = this->tiles[y + 1][x].type;
        char south_west = this->tiles[y + 1][x - 1].type;
        char south_east = this->tiles[y + 1][x + 1].type;

        if (south == '.' || south_west == '.' || south_east == '.')
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }

    std::pair<int,int> get_next_sand_tile(int x, int y)
    {
        if (y + 1 > this->upper_height_bound || x - 1 < 0 || x + 1 > this->upper_width_bound - this->lower_width_bound)
        {
            return std::make_pair(x, y);
        }
        else
        {
            char south = this->tiles[y + 1][x].type;
            char south_west = this->tiles[y + 1][x - 1].type;
            char south_east = this->tiles[y + 1][x + 1].type;

            if (south == '.')
            {
                return std::make_pair(x, y + 1);
            }
            else if (south_west == '.')
            {
                return std::make_pair(x - 1, y + 1);
            }
            else if (south_east == '.')
            {
                return std::make_pair(x + 1, y + 1);
            }
            else
            {
                return std::make_pair(x, y);
            }
        } 
    }

    int sand_simulate()
    {
        int sand_ticker = 0;
        int sand_start_x = 500 - this->lower_width_bound;
        int sand_start_y = 0;
        bool in_bounds = true;

        while(in_bounds == true)
        {               
            int curr_sand_x = sand_start_x;
            int curr_sand_y = sand_start_y;
            //this->render_map();

            if (this->sand_can_move(sand_start_x, sand_start_y) == true)
            {
                while(this->sand_can_move(curr_sand_x, curr_sand_y) == true)
                {
                    std::pair<int, int> new_pos = this->get_next_sand_tile(curr_sand_x, curr_sand_y);

                    curr_sand_x = new_pos.first;
                    curr_sand_y = new_pos.second;
                }
            }
            else
            {
                in_bounds = false;
            }

            sand_ticker++;
            this->tiles[curr_sand_y][curr_sand_x].set_sand();

            if (curr_sand_x - 1 < 0 || curr_sand_x + 1 > this->upper_width_bound - this->lower_width_bound || curr_sand_y + 1 > this->upper_height_bound)
            {
                in_bounds = false;
            }
        }

        return sand_ticker;
    }

    //Takes a 2d list of vector pairs and takes vector xn to xn+1 and turns them into '#' tiles
    void convert_vectors_to_tiles(std::vector<std::vector<std::pair<int,int>>> set_vectors)
    {

        for (auto line_vector : set_vectors)
        {
            for (auto vector = line_vector.begin(); vector != line_vector.end(); vector++)
            {
                //Draw line from current vector to next vector
                auto next_vector = vector + 1;
                if (next_vector != line_vector.end())
                {
                    int x1 = vector->first;
                    int y1 = vector->second;
                    int x2 = next_vector->first;
                    int y2 = next_vector->second;

                    int dx = x2 - x1;
                    int dy = y2 - y1;

                    if (dx != 0)
                    {
                        if (dx > 0)
                        {
                            for (int x = x1; x <= x2; x++)
                            {
                                this->set_tile_type(x - this->lower_width_bound, y1 - this->lower_height_bound, '#');
                            }
                        }
                        else
                        {
                            for (int x = x1; x >= x2; x--)
                            {
                                this->set_tile_type(x - this->lower_width_bound, y1 - this->lower_height_bound, '#');
                            }
                        }
                    }
                    else if (dy != 0)
                    {
                        if (dy > 0)
                        {
                            for (int y = y1; y <= y2; y++)
                            {
                                this->set_tile_type(x1 - this->lower_width_bound, y - this->lower_height_bound, '#');
                            }
                        }
                        else
                        {
                            for (int y = y1; y >= y2; y--)
                            {
                                this->set_tile_type(x1 - this->lower_width_bound, y - this->lower_height_bound, '#');
                            }
                        }
                    }
                }
            }
        }
    }
};