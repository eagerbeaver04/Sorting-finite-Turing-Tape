#include "file_tape.hpp"

template<std::integral T>
FileTape<T>::FileTape(const std::string& config_path, const std::string &filename) : 
    config_path(config_path), tape_filename(filename)
{
    file.open(tape_filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("Failed to open tape file!");

    read_config();
}

template <std::integral T>
void FileTape<T>::read_config()
{
    std::ifstream file(config_path);
    if(!file)
        throw std::runtime_error("Failed to open config file!");
    file >> delays;
}

template <std::integral T>
FileTape<T>::~FileTape()
{
    if (file.is_open())
        file.close();
}

template <std::integral T>
T FileTape<T>::read()
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

template <std::integral T>
void FileTape<T>::write(T &&value)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.write_delay));
    char buffer[sizeof(T)];

    std::memcpy(buffer, &value, sizeof(T));
    file.write(buffer, sizeof(buffer));
}

template <std::integral T>
void FileTape<T>::rewind()
{
    file.seekg(0);
    file.seekp(0);
}

template <std::integral T>
void FileTape<T>::move_left()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.move_delay));
    file.seekg(-static_cast<T>(sizeof(T)), std::ios::cur);
    file.seekp(-static_cast<T>(sizeof(T)), std::ios::cur);
}

template <std::integral T>
void FileTape<T>::move_right()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delays.move_delay));
    file.seekg(static_cast<T>(sizeof(T)), std::ios::cur);
    file.seekp(static_cast<T>(sizeof(T)), std::ios::cur);
}

template <std::integral T>
void FileTape<T>::clear()
{
    file.close();

    std::ofstream ofs(tape_filename, std::ios::out | std::ios::trunc);
    ofs.close();

    file.open(tape_filename, std::ios::in | std::ios::out | std::ios::binary);
}