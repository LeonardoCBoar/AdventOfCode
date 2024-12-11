#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <cmath>
#include <unordered_map>
#include <stack>

// const std::string INPUT_FILENAME = "a.txt";
const std::string INPUT_FILENAME = "input.txt";
const size_t BLINK_COUNT = 75;

inline uint64_t digit_count(const uint64_t num)
{
    return std::floor(std::log10(num) + 1);
}

uint64_t get_size_after_blinks(const uint64_t first_stone, const size_t blink_count, std::unordered_map<uint64_t, std::array<uint64_t, BLINK_COUNT>>& cache)
{
    uint64_t stone_value = first_stone;
    uint64_t sum = 0;

    for(size_t i = 0; i < blink_count; ++i)
    {
        if(stone_value == 0)
            stone_value = 1;
        else 
        {
            const uint64_t digits = digit_count(stone_value);
            const uint64_t half_digits = digits / 2;
            if(digits % 2 == 0)
            {
                const auto delimiter = static_cast<uint64_t>(std::pow(10, half_digits));
                const uint64_t second_num = stone_value % delimiter;
                stone_value /= delimiter;
                
                if(blink_count - i > 1)
                {
                    const size_t remaining_blinks = blink_count - i - 1;
                    if(!cache.contains(second_num))
                        cache[second_num] = {};

                    if(cache[second_num][remaining_blinks] == 0)
                        cache[second_num][remaining_blinks] = get_size_after_blinks(second_num, remaining_blinks, cache);

                    sum += cache[second_num][remaining_blinks];
                }
                else
                    ++sum;
            }
            else
                stone_value *= 2024;
        }

    }

    return sum + 1;

}

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


    std::stack<uint64_t> start_stones;
    std::string temp;

    while(buffer >> temp)
        start_stones.push(std::atoi(temp.c_str()));

    // std::unordered_map<uint64_t, std::array<uint64_t, BLINK_COUNT>> cache;
    std::unordered_map<uint64_t, std::array<uint64_t, BLINK_COUNT>> cache;
    uint64_t total_stones = 0;
    while(!start_stones.empty())
    {
        const uint64_t first_stone = start_stones.top();
        start_stones.pop();

        total_stones += get_size_after_blinks(first_stone, BLINK_COUNT, cache);
    }




    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "stones: " << total_stones << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;

    // for(uint64_t i = 0; i < 11; i++)
    //     std::cout << i << "has " << digit_count(i, log10) << " digits\n";
}