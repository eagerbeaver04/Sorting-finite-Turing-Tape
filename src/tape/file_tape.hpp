#pragma once

#include <thread>
#include <chrono>
#include <filesystem>
#include <cstring>
#include <concepts>
#include "../interface/i_tape.hpp"
#include "../interface/delays.hpp"

template<std::integral T>
class FileTape final : public ITape<T>
{
private:
    std::filesystem::path config_path;
    std::fstream file;
    Delays delays;
public:
    FileTape(const std::string &config_path, const std::string &filename);

    virtual ~FileTape() override;

    virtual T read() override;

    virtual void write(T&& value) override;

    virtual void rewind() override;

    virtual void move_left() override;

    virtual void move_right() override;

    virtual void read_config() override;
};
