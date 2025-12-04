#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <math.h>
#include <string_view>
#include <ranges>
#include <iomanip>

bool is_valid_number(const int64_t number)
{
    
    std::string num_string = std::to_string(number);
    if(num_string[0] == '0')
        return false;
    if(num_string.size() % 2 != 0)
        return true;
    
    const size_t half_size = num_string.size() / 2;
    std::string_view first_half{num_string.data(), half_size};
    std::string_view second_half{num_string.data() + half_size, half_size};

    for(size_t i = 0; i < half_size; ++i)
    {
        if(first_half[i] != second_half[i])
            return true;
    }
    
    return false;
}

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("test.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::string temp_line;

    uint64_t sum;
    while(std::getline(input_file, temp_line))
    {
        std::stringstream temp_line_stream(temp_line);
        for (const auto splitted : std::views::split(temp_line, ','))
        {
            std::string_view range(splitted.begin(), splitted.end());

            const size_t comma_index = range.find('-');
            std::string_view range_start = std::string_view{range.data(), comma_index};
            std::string_view range_end = std::string_view{range.data() + comma_index + 1, range.size() - comma_index - 1};;

            std::istringstream start_ss(range_start.data());
            std::istringstream end_ss(range_end.data());
            uint64_t start;
            start_ss >> start;
            uint64_t end;
            end_ss >> end;

            for(uint64_t i = start; i <= end; ++i)
            {
                if(!is_valid_number(i))
                {
                    sum += i;

                }
            }


        }
    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Sum: " << sum<< std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}