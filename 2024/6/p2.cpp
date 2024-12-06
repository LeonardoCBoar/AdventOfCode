#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <vector>
#include <unordered_set>

enum Rotation
{
    Up,
    Right,
    Down,
    Left
};

void clear_map(const uint64_t obstruction_x, const uint64_t obstruction_y, std::vector<std::string>& map, uint64_t updated_points)
{
    map[obstruction_y][obstruction_x] = '.';
    for(auto& row : map)
    {
        for(auto& point : row)
        {
            if(point == 'X')
            {
                point = '.';
                --updated_points;
                if(updated_points == 0)
                    return;
            }
        }
    }
}

void clear_visit_count(std::vector<std::vector<std::unordered_set<Rotation>>>& visit_count)
{
    for(auto& row : visit_count)
        for(auto& point : row)
            point.clear();
}

uint64_t try_with_another_obstruction(uint64_t guard_x, uint64_t guard_y, const uint64_t obstruction_x, const uint64_t obstruction_y, std::vector<std::string>& map, std::vector<std::vector<std::unordered_set<Rotation>>>& visit_count)
{
    if(map[obstruction_y][obstruction_x] != '.')
        return 0;
    
    map[obstruction_y][obstruction_x] = '#';


    uint64_t updated_points = 0;
    Rotation current_rotation = Up;
    while(true)
    {
        size_t next_x = guard_x;
        size_t next_y = guard_y;
        switch (current_rotation) 
        {
            case Up:
                --next_y;
                break;
            case Right:
                ++next_x;
                break;
            case Down:
                ++next_y;
                break;
            case Left:
                --next_x;
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
                updated_points += map[guard_y][guard_x] == '.';
                map[guard_y][guard_x] = 'X';
                if(visit_count[guard_y][guard_x].contains(current_rotation))
                {
                    clear_map(obstruction_x, obstruction_y, map, updated_points);
                    return 1;
                }

                visit_count[guard_y][guard_x].insert(current_rotation);
                guard_x = next_x;
                guard_y = next_y;
            }
        }
        else
        {
            clear_map(obstruction_x, obstruction_y, map, updated_points);
            return 0;
        }
    }

}

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::vector<std::string> map;

    std::string temp_line;
    size_t start_guard_x = SIZE_MAX;
    size_t start_guard_y = 0;
    bool guard_found = false;
    while(std::getline(input_file, temp_line))
    {
        map.push_back(temp_line);
        if(!guard_found)
        {
            ++start_guard_y;
            start_guard_x = temp_line.find("^");
            if(start_guard_x != std::string::npos)
            {
                --start_guard_y;
                guard_found = true;
            }
        }
    }
    size_t loop_count = 0;

    std::vector<std::vector<std::unordered_set<Rotation>>> visit_count;
    visit_count.resize(map.size());
    for(auto& row : visit_count)
        row.resize(map[0].size());

    for(size_t x = 0; x < map[0].size(); ++x)
    {
        for(size_t y = 0; y < map.size(); ++y)
        {
            clear_visit_count(visit_count); 
            loop_count += try_with_another_obstruction(start_guard_x, start_guard_y, x, y, map, visit_count);
        }
    }


    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "positions guarded: " << loop_count << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}