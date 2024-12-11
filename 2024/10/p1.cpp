#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <vector>
#include <queue>

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
    std::queue<Point> trailheads;

    std::string temp_line;
    size_t y = 0;
    while(std::getline(input_file, temp_line))
    {
        size_t x = 0;
        map.push_back(temp_line);
        for(const char c : temp_line)
        {
            if(c == '0')
            {
                trailheads.push({x, y});
            }

            ++x;
        }
        ++y;
    }
    input_file.close();

    uint64_t scores = 0;


    const size_t map_width = map[0].size();
    const size_t map_height = map.size();
    while(!trailheads.empty())
    {
        std::vector<Point> consumed;
        Point current_point = trailheads.front();
        trailheads.pop();
        std::queue<Point> trail;

        while(true)
        {
            const char current_value = map[current_point.y][current_point.x];
            const auto xx = current_point.x;
            const auto yy = current_point.y;
            if(current_value == '9')
            {
                bool can = true;
                for(auto& c : consumed)
                    if(c.x == xx && c.y == yy)
                        can = false;


                if(can)
                {
                    ++scores;
                    consumed.push_back({xx,yy});
                }
            }
            else
            {
                if(current_point.x - 1 < map_width && map[yy][xx - 1] == current_value + 1)
                {
                    trail.push({current_point.x - 1, current_point.y});
                }
                if(current_point.x + 1 < map_width && map[yy][xx + 1] == current_value + 1)
                {
                    trail.push({current_point.x + 1, current_point.y});
                }
                if(current_point.y - 1 < map_height && map[yy - 1][xx] == current_value + 1)
                {
                    trail.push({current_point.x, current_point.y - 1});
                }
                if(current_point.y + 1 < map_height && map[yy + 1][xx] == current_value + 1)
                {
                    trail.push({current_point.x, current_point.y + 1});
                }
            }

            if(trail.empty())
                break;

            current_point = trail.front();
            trail.pop();
        }



    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "scores: " << scores << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}