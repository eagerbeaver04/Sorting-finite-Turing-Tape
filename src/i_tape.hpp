#pragma once

#include <string>

class ITape {
public:
    virtual ~ITape() = default;

    virtual int read() = 0;
    virtual void write(int value) = 0;
    virtual void rewind() = 0;
    virtual void move_left() = 0;
    virtual void move_right() = 0;
    virtual void read_config() = 0;
};
