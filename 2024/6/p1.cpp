#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <vector>

enum Rotation
{
    Up,
    Right,
    Down,
    Left
};

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::vector<std::string> map;

    std::string temp_line;
    size_t guard_x = SIZE_MAX;
    size_t guard_y = 0;
    bool guard_found = false;
    while(std::getline(input_file, temp_line))
    {
        map.push_back(temp_line);
        if(!guard_found)
        {
            guard_y++;
            guard_x = temp_line.find("^");
            if(guard_x != std::string::npos)
            {
                guard_y--;
                guard_found = true;
            }
        }
    }

    Rotation current_rotation = Up;
    while(true)
    {
        size_t next_x = guard_x;
        size_t next_y = guard_y;
        switch (current_rotation) 
        {
            case Up:
                next_y--;
                break;
            case Right:
                next_x++;
                break;
            case Down:
                next_y++;
                break;
            case Left:
                next_x--;
                break;
            default:
                assert(false);
        }
            
        if(next_x < map[0].size() && next_y < map.size())
        {
            if(map[next_y][next_x] == '#')
                current_rotation = static_cast<Rotation>((current_rotation + 1) % 4);
            else
            {
                map[guard_y][guard_x] = 'X';
                guard_x = next_x;
                guard_y = next_y;
                map[guard_y][guard_x] = 'X';
            }
        }
        else
        {
            break;
        }
    }

    uint64_t count = 0;
    for(const auto& row : map)
    {
        for(const auto point : row)
        {
            if(point == 'X') count++;
        }
    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "positions guarded: " << count << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}