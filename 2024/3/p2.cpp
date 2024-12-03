#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <sys/types.h>

enum SequenceState
{
    UNDEFINED,
    INCREASING,
    DECREASING
};

bool is_num_string(const std::string_view string)
{
    for(const auto c : string)
        if(!std::isdigit(c))
            return false;
    return true;
}

uint64_t parse_mul(const std::string_view mul_expression)
{
    const size_t separator_pos = mul_expression.find(",");
    const std::string_view first_num_str = std::string_view(mul_expression.data(), separator_pos);
    const std::string_view second_num_str = std::string_view(mul_expression.data()+ separator_pos + 1);

    if(!is_num_string(first_num_str) || !is_num_string(second_num_str))
        return 0;

    const uint64_t first_num = std::atoi(first_num_str.data());
    const uint64_t second_num = std::atoi(second_num_str.data());


    return first_num * second_num;
}

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    uint64_t sum = 0;
    std::string input;
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    input = buffer.str();


    size_t mul_start = input.find("mul(");
    size_t mul_end = 0;
    while(mul_start != std::string::npos)
    {
        mul_end = input.find(")", mul_start);
        if(mul_end == std::string::npos)
            break;
        const size_t next_mul = input.find("mul(", mul_start + 1);
        if(next_mul != std::string::npos && next_mul < mul_end)
        {
            mul_start = next_mul;
            continue;
        }

        const size_t last_dont = input.rfind("don't()",mul_start);
        const size_t last_do = input.rfind("do()",mul_start);
        if(last_dont != std::string::npos)
        {
            const size_t do_after_dont = input.substr(last_dont, mul_start - last_dont).find("do()");
            if(do_after_dont == std::string::npos)
            {
                mul_start = next_mul;
                continue;
            }
        }

        sum += parse_mul(input.substr(mul_start + 4, mul_end - mul_start - 4));
        mul_start = next_mul;

    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "sum: " << sum << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}