#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <vector>
#include <unordered_map>

struct Point
{
    uint64_t x;
    uint64_t y;
};


int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
    {
        std::cout << "error opening file\n";
        return 1;
    }

    std::vector<std::string> map;
    std::unordered_map<char, std::vector<Point>> antenas;

    std::string temp_line;
    size_t y = 0;
    while(std::getline(input_file, temp_line))
    {
        size_t x = 0;
        map.push_back(temp_line);
        for(const char c : temp_line)
        {
            if(c != '.')
            {
                if(antenas.contains(c))
                    antenas[c].push_back({x, y});
                else
                    antenas[c] = {Point{x,y}};
            }
            ++x;
        }
        ++y;
    }
    input_file.close();

    const size_t map_width = map[0].size();
    const size_t map_height = map.size();
    uint64_t antinodes = 0;
    for(const auto& [_, points] : antenas)
    {
        for(size_t i = 0; i < points.size(); ++i)
        {
            for(size_t j = 0; j < points.size(); ++j)
            {
                if(i == j) continue;
                Point step;
                step.x = points[i].x - points[j].x;
                step.y = points[i].y - points[j].y; 

                size_t k = 0;
                while(true)
                {
                    Point possible_antinode;
                    possible_antinode.x = points[i].x + step.x * k;
                    possible_antinode.y = points[i].y + step.y * k;

                    if(possible_antinode.x >= map_width)
                        break;
                    if(possible_antinode.y >= map_height)
                        break;
                    
                    auto& index = map[possible_antinode.y][possible_antinode.x];
                    if(index != '#')
                    {
                        index = '#';
                        ++antinodes;
                    }
                    ++k;
                }
            }
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "antinodes: " << antinodes << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}