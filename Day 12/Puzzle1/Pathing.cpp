#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

class Path_node
{
    public:
        int steps = 0;
        int x;
        int y;
        int height;
        bool explored = false;
        bool start = false;
        bool goal = false;
        std::vector<Path_node *> connections;

    Path_node(int height, int x, int y, char key)
    {
        if (key == 'S') this->start = true;
        else if (key == 'E') this->goal = true;

        this->height = height;
        this->x = x;
        this->y = y;
    }

    int steps_to_goal()
    {
        std::queue<Path_node *> next_level;
        std::queue<Path_node *> current_level;
        current_level.push(this);

        int i = 0;
        while (true)
        {
            i++;
            while (current_level.size() > 0)
            {
                Path_node *node = current_level.front();
                current_level.pop();

                if (node->goal == true) return node->steps;
                for (auto connection : node->connections)
                {

                    if (connection->explored == false)
                    {
                        connection->explored = true;
                        connection->steps = node->steps + 1;
                        next_level.push(connection);
                    }
                }
            }

            if (next_level.size() == 0) return -1;
            current_level = next_level;
            next_level = std::queue<Path_node *>();
        }
    }
};

//Gets the file with weighted nodes and returns them as a vector of strings so it's 2d parseable
std::vector<std::string> get_map_vector(std::string file_name)
{
    std::ifstream file(file_name);
    std::vector<std::string> map_vector;

    for (std::string line; std::getline(file, line);)
    {
        map_vector.push_back(line);
    }

    return map_vector;
}

std::vector<std::vector<Path_node *>> make_node_map(std::vector<std::string> map_vector)
{
    //Key's index is numeric height value
    std::string key = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::vector<Path_node *>> Paths;

    int y = 0;
    for (auto string : map_vector)
    {
        int x = 0;
        std::vector<Path_node *> temp;
        for (auto point : string)
        {
            if (point == 'S') temp.push_back(new Path_node(key.find('a'), x, y, point));
            else if (point == 'E') temp.push_back(new Path_node(key.find('z'), x, y, point));
            else temp.push_back(new Path_node(key.find(point), x, y, point));
            x++;
        }
        Paths.push_back(temp);
        y++;
    }

    return Paths;
}

bool is_reachable_neighbor(int y_node, int x_node, int y, int x, std::vector<std::vector<Path_node *>> path_map, int max_dif)
{
    int difference = abs(x_node - x) + abs(y_node - y);

    //math vector difference less than 2, x and y are in bounds of the map, and the neighbor is not itself.
    if (difference < max_dif && x >= 0 && y >= 0 && x < path_map[y].size() && y < path_map.size() && !(x == x_node && y == y_node))
    {
        //Must be only 1 higher or lower than current node
        if (abs(path_map[y_node][x_node]->height - path_map[y][x]->height) < 2 || path_map[y_node][x_node]->height > path_map[y][x]->height)
        {
            return true;
        }     
    }
    return false;
}

std::vector<std::vector<Path_node *>> get_connections(std::vector<std::vector<Path_node *>> path_map, int max_dif)
{
    //First two for loops go through path_map's nodes to create a "current node"
    for (int y_node = 0; y_node < path_map.size(); y_node++)
    {
        for (int x_node = 0; x_node < path_map[y_node].size(); x_node++)
        {
            //Second two iterate through the neighboring nodes for the "current node"
            for (int x = x_node - 1; x < x_node + 2; x++)
            {
                for(int y = y_node - 1; y < y_node + 2; y++)
                {
                    //In try-catch in case of out-of-bounds
                    try
                    {
                        
                        if (is_reachable_neighbor(y_node, x_node, y, x, path_map, max_dif))
                        {
                            path_map[y_node][x_node]->connections.push_back(path_map[y][x]);
                        }
                    }

                    catch (...)
                    {
                        continue;
                    }
                }
            }
        }
    }

    return path_map;
}


int main()
{   
    //                                                                                 SET INPUT VVVVV
    std::vector<std::vector<Path_node *>> path_map = get_connections(make_node_map(get_map_vector("input.txt")), 2);

    //Find starting node, member variable "start" is set to true
    Path_node *start;
    for (auto row : path_map)
    {
        for (auto node : row)
        {
            if (node->start == true) start = node;
        }
    }

    std::cout << "Steps to goal: " << start->steps_to_goal() << std::endl;

}


