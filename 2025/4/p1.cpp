#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <math.h>
#include <string_view>
#include <ranges>
#include <iomanip>

inline int is_forklift(const std::vector<std::string>& matrix, const size_t x, const size_t y)
{
    return matrix[y][x] == '@';
}


int main(int argc, char** argv)
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file(argv[1]);

    if(!input_file.is_open())
        std::cout << "error opening file\n";


    std::vector<std::string> matrix;
    std::string temp_line;

    uint64_t acessable_rolls = 0;
    while(std::getline(input_file, temp_line))
    {
        matrix.push_back(temp_line);
    }

    const size_t size_x = matrix[0].size();
    const size_t size_y = matrix.size();

    for(size_t y = 0; y < matrix.size(); ++y)
    {
        for(size_t x = 0; x < size_x; ++x)
        {
            if(is_forklift(matrix, x, y))
            {
                size_t adjacent_rolls_count = 0;
                for(int64_t i = -1; i < 2; ++i)
                {
                    if(y + i >= size_y)
                        continue;
                    for(int64_t j = -1; j < 2; ++j)
                    {
                        if(x + j >= size_x)
                            continue;
                        
                        adjacent_rolls_count += is_forklift(matrix, x+j, y+i);
                    }
                }
                adjacent_rolls_count -= 1; // Discount itself
                if(adjacent_rolls_count < 4)
                    ++acessable_rolls;
            }
        }
    }


    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rolls accessable: " << acessable_rolls << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}