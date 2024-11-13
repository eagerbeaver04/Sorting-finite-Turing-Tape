#pragma once

#include <thread>
#include <chrono>
#include <filesystem>

#include "i_tape.hpp"
#include "delays.hpp"

class FileTape final : public ITape
{
private:
    std::filesystem::path config_path;
    std::fstream file;
    Delays delays;

public:
    FileTape(const std::string &config_path, const std::string &filename);

    virtual ~FileTape() override;

    virtual int read() override;

    virtual void write(int value) override;

    virtual void rewind() override;

    virtual void move_left() override;

    virtual void move_right() override;

    virtual void read_config() override;
};
