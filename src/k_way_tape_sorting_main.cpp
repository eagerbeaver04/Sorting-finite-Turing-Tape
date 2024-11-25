#include <iostream>
#include "tape/k_way_tape_sorter.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Incorrect format!";
        return 1;
    }

    std::cout << "Enter input filename:" << std::endl;
    std::string input_file;
    std::cin >> input_file;

    std::cout << "Enter output filename:" << std::endl;
    std::string output_file;
    std::cin >> output_file;

    std::cout << "Do you want to demonstrate input and ouput content? Enter 1 if you want and 0 if you doesn't" << std::endl;
    int f;
    std::cin >> f;
    KWayTapeSorter<FileTape, int> k_way_sorter(std::string(argv[1]),
        input_file, output_file, std::stoul(argv[2]), 10);
        
    k_way_sorter.sort();
    if(f == 1)
    {
        std::cout << "input_file: ";
        FileTape<int> input_tape(std::string(argv[1]), input_file);
        std::optional<int> val;
        std::vector<int> data;
        while(true)
        {
            val = input_tape.read();
            if(!val)
                break;
            std::cout << val.value() << " ";
            data.push_back(val.value());
        }
        std::cout << std::endl;

        std::sort(data.begin(), data.end());
        std::cout << "sorted data: ";
        for(int el : data)
            std::cout << el << " ";
        std::cout << std::endl;

        std::cout << "output_file: ";
        FileTape<int> output_tape(std::string(argv[1]), output_file);
        while (true)
        {
            val = output_tape.read();
            if (!val)
                break;
            std::cout << val.value() << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}