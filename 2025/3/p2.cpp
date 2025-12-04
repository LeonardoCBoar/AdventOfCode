#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <math.h>
#include <string_view>
#include <ranges>
#include <iomanip>


int main(int argc, char** argv)
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file(argv[1]);

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::string temp_line;

    uint64_t sum = 0;
    while(std::getline(input_file, temp_line))
    {
        std::array<uint64_t, 12> highest;

        uint64_t highest_index = -1;
        for(size_t i = 0; i < 12; i++)
        {
            uint64_t current_highest = 0;
            for(uint64_t j = highest_index + 1; j < temp_line.size() - (11 - i) ; ++j)
            {
                const uint64_t number = (uint64_t) temp_line[j] - '0';
                if(number > current_highest)
                {
                    current_highest  = number;
                    highest_index = j;
                }
            }
            highest[i] = current_highest;

        }

        for(size_t i = 0; i < 12; ++i)
        {
            sum += highest[i] * std::pow(10, 11-i);
        }
        std::cout << std::endl;

    }


    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Joltage: " << sum<< std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}