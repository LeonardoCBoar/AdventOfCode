#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <vector>

// const std::string INPUT_FILENAME = "a.txt";
const std::string INPUT_FILENAME = "input.txt";
const size_t BLINK_COUNT = 25;



int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file(INPUT_FILENAME);

    if(!input_file.is_open())
    {
        std::cout << "error opening file\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << input_file.rdbuf();


    std::vector<std::pair<std::string, uint64_t>> stones;
    std::string temp;

    while(buffer >> temp)
        stones.push_back({temp, std::atoi(temp.c_str())});

    for(size_t i = 0; i < BLINK_COUNT; i++)
    {
        const size_t stones_length = stones.size();
        for(size_t j = 0; j < stones_length; j++)
        {
            auto& stone = stones[j];
            if(stone.second == 0) stone = {"1", 1};
            else if(stone.first.length() % 2 == 0)
            {
                const size_t half_len = stone.first.length() / 2;
                std::pair<std::string, uint64_t> new_stone = {stone.first.substr(half_len), std::atoi(stone.first.c_str() + half_len)};
                if(new_stone.first[0] == '0')
                    new_stone.first = std::to_string(new_stone.second);
                stone.first.resize(half_len);
                stone.second = std::atoi(stone.first.c_str());
                stones.push_back(std::move(new_stone));
            }
            else
            {
                stone.second *= 2024;
                stone.first = std::to_string(stone.second);
            }

        }
        // std::cout << "iteration: " << i << " stones: " << stones.size() << std::endl;
    }




    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "stones: " << stones.size() << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}