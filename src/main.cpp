#include <iostream>
#include "tape/tape_sorter.hpp"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Incorrect format!";
        return 1;
    }
    TapeSorter<FileTape, int> sorter(std::string(argv[1]), "file1.bin", "file2.bin", 10);
    return 0;
}