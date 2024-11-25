#include <iostream>
#include "tape/tape_sorter.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
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

    TapeSorter<FileTape, int> sorter(std::string(argv[1]),
        input_file, output_file, std::stoul(argv[2]));

    sorter.sort();

    return 0;
}