#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>
#include <optional>
#include <algorithm>

enum SequenceState
{
    UNDEFINED,
    INCREASING,
    DECREASING
};

bool validate_line(const std::vector<int32_t>& line_values,  const std::optional<int32_t> ignored_index);

bool try_validate_ignoring_adjacent_values(const std::vector<int32_t>& line_values, const int32_t error_index)
{
    for(int32_t j = std::max(error_index - 2, 0); j < std::min(error_index + 1, (int32_t)line_values.size()); j++)
    {
        const bool result = validate_line(line_values, std::make_optional(j));
        if(result)
            return true;
    }
    return false;
}

bool validate_line(const std::vector<int32_t>& line_values,  const std::optional<int32_t> ignored_index)
{
    SequenceState sequenceState = SequenceState::UNDEFINED;
    for(size_t i = 1; i < line_values.size(); i++)
    {
        int32_t diff;
        if(ignored_index.has_value())
        {
            if(i == ignored_index.value())
                continue;
            else if(i == ignored_index.value() + 1)
            {
                if(i > 1)
                    diff = line_values[i] - line_values[i - 2];
                else
                    continue;
            }
            else
                diff = line_values[i] - line_values[i - 1];
        }
        else
            diff = line_values[i] - line_values[i - 1];


        if(diff == 0 || abs(diff) > 3)
        {
            if(ignored_index.has_value())
                return false;
            else
            {
                return try_validate_ignoring_adjacent_values(line_values, i);
            }
        }

        if(diff > 0)
        {
            if(sequenceState == SequenceState::DECREASING)
            {
                if(ignored_index.has_value())
                    return false;
                else
                {
                    return try_validate_ignoring_adjacent_values(line_values, i);
                }
            }
            sequenceState = SequenceState::INCREASING;
        }
        else
        {
            if(sequenceState == SequenceState::INCREASING)
            {
                if(ignored_index.has_value())
                    return false;
                else
                {
                    return try_validate_ignoring_adjacent_values(line_values, i);
                }
            }
            sequenceState = SequenceState::DECREASING;
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

    uint64_t safe_count =  0;
    std::string temp_line;
    while(std::getline(input_file, temp_line))
    {
        std::stringstream temp_line_stream(temp_line);
        std::string temp_word;

        std::vector<int32_t> line_values;
        while(temp_line_stream >> temp_word)
        {
            const int32_t current_value = std::atoi(temp_word.c_str());
            line_values.push_back(current_value);
        }

        if(validate_line(line_values, std::nullopt))
            safe_count++;
    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "safe count: " << safe_count << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}