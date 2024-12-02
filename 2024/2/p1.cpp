#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

enum SequenceState
{
    UNDEFINED,
    INCREASING,
    DECREASING
};

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("a.txt");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    uint64_t safe_count =  0;
    std::string temp_line;
    while(std::getline(input_file, temp_line))
    {
        std::stringstream temp_line_stream(temp_line);
        SequenceState sequenceState = SequenceState::UNDEFINED;

        std::string temp_word;
        temp_line_stream >> temp_word;
        int32_t last_value = std::atoi(temp_word.c_str());
        while(temp_line_stream >> temp_word)
        {
            const int32_t current_value = std::atoi(temp_word.c_str());
            const int32_t diff = current_value - last_value;
            if(diff == 0 || abs(diff) > 3)
                goto unsafe;

            if(diff > 0)
            {
                if(sequenceState == SequenceState::DECREASING)
                    goto unsafe;
                sequenceState = SequenceState::INCREASING;
            }
            else
            {
                if(sequenceState == SequenceState::INCREASING)
                    goto unsafe;
                sequenceState = SequenceState::DECREASING;
            }
            last_value = current_value;
        }
        safe_count++;
        unsafe:
            continue;
    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "safe count: " << safe_count << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}