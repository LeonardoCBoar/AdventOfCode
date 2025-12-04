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

    int64_t sum = 0;
    while(std::getline(input_file, temp_line))
    {
        int64_t highest = 0;
        size_t highest_index = 0;
        int64_t second_highest = 0;

        for(size_t i = 0; i < temp_line.size() - 1 ; ++i)
        {
            const int64_t number = (int64_t) temp_line[i] - '0';
            if(number > highest)
            {
                highest = number;
                highest_index = i;
            }
        }
        for(size_t i = highest_index + 1; i < temp_line.size(); ++i)
        {
            const int64_t number = (int64_t) temp_line[i] - '0';
            if(number > second_highest)
            {
                second_highest = number;
            }
        }

        int64_t joltage = highest * 10 + second_highest;
        sum += joltage;

        std::cout << "highest: " << highest << " second highest: " << second_highest << std::endl;
    }


    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Joltage: " << sum<< std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}