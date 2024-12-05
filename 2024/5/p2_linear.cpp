#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>

bool is_ordered(const std::vector<uint64_t>& pages, const std::unordered_map<uint64_t, std::unordered_set<uint64_t>>& order_rules)
{
    for(size_t i = 1; i < pages.size(); i++)
    {
        const uint64_t current_page = pages[i];
        if(!order_rules.contains(current_page))
            continue;

        for(size_t j = i - 1; j < SIZE_MAX; j--)
        {
            if(order_rules.at(current_page).contains(pages[j]))
            {
                return false;
            }

        }
    }
    return true;
}

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    uint64_t middle_sum = 0;
    std::unordered_map<uint64_t, std::unordered_set<uint64_t>> order_rules;
    bool section2 = false;

    std::string temp_line;
    while(std::getline(input_file, temp_line))
    {
        if(!section2)
        {
            if(temp_line.empty())
            {
                section2 = true;
                continue;
            }

            const std::string_view first_str(temp_line.c_str(), 2);
            const std::string_view second_str(temp_line.c_str() + 3, 2);

            const uint64_t first_num = std::atoi(first_str.data());
            const uint64_t second_num = std::atoi(second_str.data());
            if(order_rules.contains(first_num))
                order_rules.at(first_num).insert(second_num);
            else
                order_rules[first_num] = {second_num};

        }
        else
        {
            std::vector<uint64_t> pages;
            for(size_t i = 0; i < temp_line.length(); i+=3)
            {
                const std::string_view page{temp_line.c_str() + i, 2};
                pages.emplace_back(std::atoi(page.data()));
            }

            const bool ordered = is_ordered(pages, order_rules);
            if(!ordered)
            {
                auto sort_function = [&order_rules](uint64_t a, uint64_t b)
                {
                    if(order_rules.contains(a) && order_rules.at(a).contains(b)) 
                        return true;
                   return false;
                };

                std::sort(pages.begin(), pages.end(), sort_function);
                middle_sum += pages[std::floor(pages.size() / 2.0)];
            }
        }
    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "middle sum: " << middle_sum << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}