#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <chrono>

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::unordered_map<uint32_t, uint32_t> first_collumn_ocurrences;
    std::unordered_map<uint32_t, uint32_t> last_collumn_ocurrences;

    std::string temp;
    while(std::getline(input_file, temp))
    {
        const size_t first_num_limit = temp.find(" ");
        const size_t last_num_limit = temp.rfind(" ");

        const auto first_num_str = temp.substr(0, first_num_limit);
        const auto last_num_str = temp.substr(last_num_limit + 1, temp.size());

        const uint32_t first_num = std::atoi(first_num_str.c_str());
        const uint32_t last_num = std::atoi(last_num_str.c_str());

        if(first_collumn_ocurrences.contains(first_num))
            first_collumn_ocurrences[first_num]++;
        else
            first_collumn_ocurrences[first_num] = 1;

        if(last_collumn_ocurrences.contains(last_num))
            last_collumn_ocurrences[last_num]++;
        else
            last_collumn_ocurrences[last_num] = 1;
    }
    input_file.close();

    uint64_t similarity_score = 0;
    for (const auto&[num, ocurrences] : first_collumn_ocurrences)
        similarity_score += num * ocurrences * last_collumn_ocurrences[num];

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "similarity score: " << similarity_score<< std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}