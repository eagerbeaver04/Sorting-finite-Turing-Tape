#pragma once

#include <string>
#include <fstream>

struct Delays
{
    int read_delay;
    int write_delay;
    int move_delay;

    friend std::ifstream& operator>>(std::ifstream &in, Delays &delay)
    {
        in >> delay.read_delay;
        in >> delay.write_delay;
        in >> delay.move_delay;
        return in;
    }
};