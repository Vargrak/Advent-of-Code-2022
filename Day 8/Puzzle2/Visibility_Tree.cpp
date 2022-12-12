#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

int visible_north(std::vector<std::string> map, int x, int y, int view_count)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = y - 1; traverse >= 0; traverse--)
    {
        //std::cout << "Traversing north: " << traverse << std::endl;
        if(map[traverse][x] >= map[y][x])
        {
            //std::cout << "north false" << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            view_count++;
            return view_count;
        }
        view_count++;
    }
    return view_count;
}


int visible_south(std::vector<std::string> map, int x, int y, int view_count)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = y + 1; traverse < map.size(); traverse++)
    {
        //std::cout << "Traversing south: " << traverse << std::endl;
        if(map[traverse][x] >= map[y][x])
        {

            //std::cout << "south false" << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            view_count++;
            return view_count;
        }
        view_count++;
    }
    return view_count;
}

int visible_east(std::vector<std::string> map, int x, int y, int view_count)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = x + 1; traverse < map[y].size(); traverse++)
    {
        //std::cout << "Traversing east: " << traverse << std::endl;
        if(map[y][traverse] >= map[y][x])
        {
            //std::cout << "east false " << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            view_count++;
            return view_count;
        }
        view_count++;
    }
    return view_count;
}

int visible_west(std::vector<std::string> map, int x, int y, int view_count)
{
    //std::cout << "Tree size: " << map[y][x] << std::endl;
    for(int traverse = x - 1; traverse >= 0; traverse--)
    {
        //std::cout << "Traversing west: " << traverse << std::endl;
        if(map[y][traverse] >= map[y][x])
        {
            //std::cout << "west false" << "Tree: " << map[y][x] << " less than " << map[y][traverse] << std::endl;
            view_count++;
            return view_count;
        }
        view_count++;
    }
    return view_count;
}


int get_best_visibility(std::vector<std::string> map)
{
    int best_view_count = 0;

    int x = 0;
    int y = 0;
    for(auto itr = map.begin(); itr != map.end(); ++itr)
    {
        for(auto tree = itr->begin(); tree != itr->end(); ++tree)
        {
            int view_count = 0;
            view_count = (visible_north(map, x, y, 0) * visible_east(map, x, y, 0) * visible_south(map, x, y, 0) * visible_west(map, x, y, 0));

            if(view_count > best_view_count)
            {
                best_view_count = view_count;
            }

            x++;
        }
        y++;
        x = 0;
    }
    return best_view_count;
}

int main()
{
    int scenic_score = 0;
    std::ifstream input_file("input.txt");
    std::vector<std::string> map;

    for(std::string line; std::getline(input_file, line);)
    {
        map.push_back(line);
    }

    scenic_score = get_best_visibility(map);
    std::cout << scenic_score << std::endl;

    return 0;
}