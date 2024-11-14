#pragma once

#include <string>

template<typename T>
class ITape {
public:
    virtual ~ITape() = default;

    virtual T read() = 0;
    virtual void write(T&& value) = 0;
    virtual void rewind() = 0;
    virtual void move_left() = 0;
    virtual void move_right() = 0;
    virtual void read_config() = 0;
    virtual void clear() = 0;
};
