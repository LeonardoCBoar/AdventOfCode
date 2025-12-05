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
    std::vector<std::pair<uint64_t, int64_t>> ranges;

    uint64_t fresh_ingredientes = 0;
    while(std::getline(input_file, temp_line))
    {
        if(temp_line.size() == 0)
            break;

        size_t separator_index = temp_line.find('-');
        std::string_view before_separator{temp_line.data(), separator_index};
        std::string_view after_separator{temp_line.data() + separator_index + 1, temp_line.size() - separator_index - 1};

        uint64_t start = std::atol(before_separator.data());
        uint64_t end = std::atol(after_separator.data());
        std::cout << start << " - " << end << std::endl;

        ranges.emplace_back(start, end);
    }

    while(std::getline(input_file, temp_line))
    {
        const uint64_t ingredient_id = std::atol(temp_line.data());

        bool is_fresh = false;
        for(const auto& [start, end] : ranges)
        {
            if(ingredient_id >= start && ingredient_id <= end)
            {
                is_fresh = true;
                break;
            }
        }
        // std::cout << ingredient_id << " is fresh: " << is_fresh << std::endl;;
        fresh_ingredientes += static_cast<uint64_t>(is_fresh);
    }




    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Fresh ingredients: " << fresh_ingredientes << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}