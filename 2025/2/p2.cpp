#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <math.h>
#include <string_view>
#include <ranges>
#include <iomanip>
#include <set>

std::set<std::string> invalid_sequences;

bool is_sequence_repeated(const std::string_view sequence,const size_t first_sequence_end, const std::string& string)
{
    size_t sequence_start;
    size_t sequence_end;
    while(true)
    {
        sequence_start = sequence_end;
        sequence_end = sequence_start + sequence.size();
        if(sequence_end > string.size())
            return false;

        std::string_view next_possible_ocurrence{string.data() + sequence_start, string.data() + sequence_end};
        if(next_possible_ocurrence.compare(sequence) == 0)
        {
            if(sequence_end == string.size())
                return true;
            else
                continue;
        }
        else
            return false;

    }
    return true;
}

bool test_repeated_sequences(const int64_t number)
{
    
    std::string num_string = std::to_string(number);
    if(num_string[0] == '0')
        return false;
    
    const size_t half_size = num_string.size() / 2;
    for(size_t sequence_size = 1; sequence_size <= num_string.size()/2; ++sequence_size)
    {
        size_t sequence_end = sequence_size;
        for(size_t sequence_start = 0; sequence_end <= num_string.size() / 2; ++sequence_start, sequence_end = sequence_start + sequence_size)
        {
            
            std::string_view sequence{num_string.data() + sequence_start, num_string.data() + sequence_end};
            if(is_sequence_repeated(sequence, sequence_end,num_string))
            {
                return true;
            }
        }
    }

    return false;
}

int main(int argc, char** argv)
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file(argv[1]);

    if(!input_file.is_open())
        std::cout << "error opening file\n";
    
    std::string temp_line;

    uint64_t sum = 0;
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
                if(test_repeated_sequences(i))
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