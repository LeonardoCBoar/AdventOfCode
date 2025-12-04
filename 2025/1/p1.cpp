#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

int main()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::ifstream input_file("input");

    if(!input_file.is_open())
        std::cout << "error opening file\n";

    std::string temp_line;
    int64_t pos = 50;
    size_t zero_count = 0;

    while(std::getline(input_file, temp_line))
    {
        std::stringstream temp_line_stream(temp_line);
        char direction;
        int64_t amount;

        temp_line_stream >> direction;
        temp_line_stream >> amount;

        if(direction == 'L')
        {
            pos -= amount;
            if(pos < 0)
            {
                pos = pos + ((abs(pos)/100) + 1) * 100;
            }
        }
        else
        {
            pos += amount;
        }
            pos = pos % 100;


        if(pos == 0)
        {
            zero_count++;

        }


    }

    input_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Password: " << zero_count << std::endl;
    std::cout << "calculated in: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / 1000.0 << "[us]" << std::endl;
}