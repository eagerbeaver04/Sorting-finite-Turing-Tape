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
    const std::filesystem::path config_path;
    const std::filesystem::path tape_filename;
    std::fstream file;
    Delays delays;
public:
    FileTape(const std::string &config_path, const std::string &filename) : 
        config_path(config_path), tape_filename(filename)
    {
        file.open(tape_filename, std::ios::in | std::ios::out | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Failed to open tape file!");

        read_config();
    }

    virtual ~FileTape() override
    {
        if (file.is_open())
            file.close();
    }
    virtual T read() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delays.read_delay));
        T value;
        char buffer[sizeof(T)];

        file.read(buffer, sizeof(buffer));
        if (file.gcount() != sizeof(T))
            throw std::runtime_error("Failed to read full integer from file!");

        std::memcpy(&value, buffer, sizeof(T));
        return value;
    }
    virtual void write(const T& value) override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delays.write_delay));
        char buffer[sizeof(T)];

        std::memcpy(buffer, &value, sizeof(T));
        file.write(buffer, sizeof(buffer));
    }
    virtual void rewind() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delays.rewind_delay));
        file.clear();
        file.seekg(0);
        file.seekp(0);
    }
    virtual void move_left() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delays.move_delay));
        file.clear();
        file.seekg(-static_cast<T>(sizeof(T)), std::ios::cur);
        file.seekp(-static_cast<T>(sizeof(T)), std::ios::cur);
    }
    virtual void move_right() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delays.move_delay));
        file.clear();
        file.seekg(static_cast<T>(sizeof(T)), std::ios::cur);
        file.seekp(static_cast<T>(sizeof(T)), std::ios::cur);
    }
    virtual void read_config() override
    {
        std::ifstream file(config_path);
        if (!file)
            throw std::runtime_error("Failed to open config file!");
        file >> delays;
    }
    virtual bool is_eof() override
    {
        return file.eof();
    }
};
