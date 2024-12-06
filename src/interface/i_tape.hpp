#pragma once

#include <string>
#include <optional>

template<typename T>
class ITape {
public:
    virtual ~ITape() = default;

    virtual std::optional<T> read() = 0;
    virtual void write(const T& value) = 0;
    virtual void rewind() = 0;
    virtual void move_left() = 0;
    virtual void move_right() = 0;
    virtual void read_config(const std::string&) = 0;
    virtual bool is_eof() = 0;
};

template <template <typename> typename T, typename N>
concept Tape = std::is_base_of_v<ITape<N>, T<N>>;