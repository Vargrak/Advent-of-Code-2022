#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

bool is_edge(std::vector<std::string> map, int x, int y)
{
    if(x == 0 || x == map[y].size() - 1) return true;
    if(y == 0 || y == map.size() - 1) return true;
    return false;
}

bool visible_north(std::vector<std::string> map, int x, int y)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = y - 1; traverse >= 0; traverse--)
    {
        //std::cout << "Traversing north: " << traverse << std::endl;
        if(map[traverse][x] >= map[y][x])
        {
            //std::cout << "north false" << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            return false;
        }
    }
    return true;
}

bool visible_south(std::vector<std::string> map, int x, int y)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = y + 1; traverse < map.size(); traverse++)
    {
        //std::cout << "Traversing south: " << traverse << std::endl;
        if(map[traverse][x] >= map[y][x])
        {

            //std::cout << "south false" << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            return false;
        }
    }
    return true;
}

bool visible_east(std::vector<std::string> map, int x, int y)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = x + 1; traverse < map[y].size(); traverse++)
    {
        //std::cout << "Traversing east: " << traverse << std::endl;
        if(map[y][traverse] >= map[y][x])
        {
            //std::cout << "east false " << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            return false;
        }
    }
    return true;
}

bool visible_west(std::vector<std::string> map, int x, int y)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = x - 1; traverse >= 0; traverse--)
    {
        //std::cout << "Traversing west: " << traverse << std::endl;
        if(map[y][traverse] >= map[y][x])
        {
            //std::cout << "west false" << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            return false;
        }
    }
    return true;
}


int get_number_of_visible_trees(std::vector<std::string> map)
{
    int visible_trees = 0;
    int x = 0;
    int y = 0;
    for(auto itr = map.begin(); itr != map.end(); ++itr)
    {
        for(auto tree = itr->begin(); tree != itr->end(); ++tree)
        {
            //std::cout << "Current tree size: " << *tree << " at x: " << x << " y: " << y << std::endl;
            if(is_edge(map, x, y) || visible_north(map, x, y) || visible_south(map, x, y) || visible_east(map, x, y) || visible_west(map, x, y))
            {
                visible_trees++;
                //std::cout << "Visible tree [" << y << "][" << x << "]" << " with size: " << map[y][x] << std::endl;
            }
            x++;
        }
        y++;
        x = 0;
    }
    return visible_trees;
}

int main()
{
    int visible_trees = 0;
    std::ifstream input_file("input.txt");
    std::vector<std::string> map;

    for(std::string line; std::getline(input_file, line);)
    {
        map.push_back(line);
    }

    visible_trees = get_number_of_visible_trees(map);
    std::cout << visible_trees << std::endl;

    return 0;
}