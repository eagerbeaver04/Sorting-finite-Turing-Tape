#include "file_tape.hpp"

FileTape::FileTape(const std::string &config_path, const std::string &filename) : config_path(config_path)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("Failed to open tape file!");

    read_config();
}

void FileTape::read_config()
{
    std::ifstream file(config_path);
    if(!file)
        throw std::runtime_error("Failed to open config file!");
    file >> delays;
}

FileTape::~FileTape() 
{
    if (file.is_open())
        file.close();
}

int FileTape::read()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.read_delay));
    int value;
    file.read(reinterpret_cast<char *>(&value), sizeof(value));
    return value;
}

void FileTape::write(int value) 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.write_delay));
    file.write(reinterpret_cast<const char *>(&value), sizeof(value));
}

void FileTape::rewind() 
{
    file.seekg(0);
    file.seekp(0);
}

void FileTape::move_left() 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.move_delay));
    file.seekg(-static_cast<int>(sizeof(int)), std::ios::cur);
    file.seekp(-static_cast<int>(sizeof(int)), std::ios::cur);
}

void FileTape::move_right() 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.move_delay));
    file.seekg(static_cast<int>(sizeof(int)), std::ios::cur);
    file.seekp(static_cast<int>(sizeof(int)), std::ios::cur);
}