#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <math.h>
#include <string_view>
#include <ranges>
#include <iomanip>
#include <unordered_set>
#include <vector>



int main(int argc, char** argv)
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file(argv[1]);

    if(!input_file.is_open())
        std::cout << "error opening file\n";


    std::string temp_line;


    std::vector<std::pair<int64_t, int64_t>> ranges;

    while(std::getline(input_file, temp_line))
    {
        if(temp_line.size() == 0)
            break;

        size_t separator_index = temp_line.find('-');
        std::string_view before_separator{temp_line.data(), separator_index};
        std::string_view after_separator{temp_line.data() + separator_index + 1, temp_line.size() - separator_index - 1};

        uint64_t start = std::atol(before_separator.data());
        uint64_t end = std::atol(after_separator.data());

        ranges.emplace_back(start, end);
    }

    while(true)
    {
        loop_start:

        for(size_t i = 0; i < ranges.size(); ++i)
        {
            auto& range = ranges[i];
            for(size_t j = 0; j < ranges.size(); ++j)
            {
                if(i == j) continue;
                auto& other_range = ranges[j];
                if(range.first > other_range.second || range.second < other_range.first) // Outside range
                    continue;

                if(range.first >= other_range.first && range.second <= other_range.second) // Inside other range
                {
                    ranges.erase(ranges.begin() + i);
                    goto loop_start;
                }
                if(range.first <= other_range.first && range.second >= other_range.second) // Other range inside
                {
                    ranges.erase(ranges.begin() + j);
                    goto loop_start;
                }
                if(range.first <= other_range.first && range.second <= other_range.second) // Starts before and ends inside other
                {
                    other_range.first = range.first;
                    ranges.erase(ranges.begin() + i);
                    goto loop_start;
                }
                if(range.first >= other_range.first && range.second >= other_range.second) // Starts inside and ends after other
                {
                    other_range.second = range.second;
                    ranges.erase(ranges.begin() + i);
                    goto loop_start;
                }
            }
        }

        break;

    }
    
    uint64_t fresh_ids = 0;
    for(const auto& [start, end] : ranges)
    {
        fresh_ids += (end - start) + 1;
    }


    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Fresh ids: " << fresh_ids << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}