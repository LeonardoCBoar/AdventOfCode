#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::vector<uint32_t> first_collumn;
    std::vector<uint32_t> last_collumn;

    std::string temp;
    while(std::getline(input_file, temp))
    {
        const size_t first_num_limit = temp.find(" ");
        const size_t last_num_limit = temp.rfind(" ");

        const auto first_num_str = temp.substr(0, first_num_limit);
        const auto last_num_str = temp.substr(last_num_limit + 1, temp.size());

        first_collumn.push_back(std::atoi(first_num_str.c_str()));
        last_collumn.push_back(std::atoi(last_num_str.c_str()));
    }
    input_file.close();

    std::sort(first_collumn.begin(), first_collumn.end());
    std::sort(last_collumn.begin(), last_collumn.end());

    uint64_t diff_sum = 0;
    for(size_t i = 0; i < first_collumn.size(); ++i)
        diff_sum += abs(first_collumn[i] - last_collumn[i]);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "diff sum: " << diff_sum << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}