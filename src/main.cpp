#include <iostream>
#include "tape/tape_sorter.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Incorrect format!";
        return 1;
    }
    TapeSorter<FileTape, int> sorter(std::string(argv[1]), 
        "files/input.bin", "files/output.bin", std::stoul(argv[2]));
    sorter.sort();
    return 0;
}