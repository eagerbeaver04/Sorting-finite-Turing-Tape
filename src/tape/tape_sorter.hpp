#pragma once

#include <filesystem>
#include <array>
#include <vector>
#include <ranges>
#include <cmath>
#include <algorithm>
#include "file_tape.hpp"

namespace utils
{
    template <std::random_access_iterator I, std::sentinel_for<I> S>
        requires std::sortable<I>
    void merge_sort(I first, S last)
    {
        if (last - first > 1)
        {
            I middle{first + (last - first) / 2};
            merge_sort(first, middle);
            merge_sort(middle, last);
            std::ranges::inplace_merge(first, middle, last);
        }
    }

    void create_file_if_not_exist(const std::string &file_path)
    {
        if (!std::filesystem::exists(file_path))
        {
            std::ofstream outfile(file_path);
            outfile.close();
        }
    }
}

template<template<typename> typename T, typename N>
concept Tape = std::is_base_of_v<ITape<N>, T<N>>;

template <template <typename> typename T, typename N>
    requires(Tape<T, N>)
class TapeSorter
{
private:
    std::string config_path;
    int M;
    int chunk_size;
    int chunks_number;
    T<N> input_tape;
    T<N> output_tape;
    std::array<T<N>, 2> tmp_tapes{};
    std::vector<N> loaded_data{};

    void prepare_tmp_tapes()
    {
        for(size_t i =0; i < tmp_tapes.size(); ++i)
        {
            std::string file_path = "tmp/tmp_line_" + std::to_string(i) + ".bin";
            utils::create_file_if_not_exist(file_path);
            tmp_tapes[i] = std::move(T<N>(config_path, file_path));
        }
    }

    void merge_chunks(T<N>& input_tmp_tape, T<N>& output_tmp_tape)
    {
        input_tmp_tape.rewind();
        output_tmp_tape.rewind();
        int pos = 0;
        N val;
        bool f = false;
        while(pos < loaded_data.size())
        {
            if(!f)
            {
                val = input_tmp_tape.read();
                if (input_tmp_tape.is_eof())
                    break;
                f = true;
            }
            if(val >= loaded_data[pos])
            {
                output_tmp_tape.write(val);
                f = false;
            }
            else
            {
                output_tmp_tape.write(loaded_data[pos]);
                ++pos;
            }
        }
        for (int i : std::ranges::iota_view{pos, loaded_data.size()})
            output_tmp_tape.write(loaded_data[i]);
        while (!input_tmp_tape.is_eof())
        {
            val = input_tmp_tape.read();
            if (!input_tmp_tape.is_eof())
                output_tmp_tape.write(val);
        }
    }

    void read_chunk()
    {
        loaded_data.clear();
        N val;
        for (int i : std::ranges::iota_view{0, chunk_size})
        {
            val = input_tape.read();
            if(input_tape.is_eof())
                break;
            loaded_data.push_back(val);
        }
    }

    void save_chunk_in_tape(T<N>& tape)
    {
        tape.rewind();
        for(auto&& el : loaded_data)
            tape.write(el);
    }
public:
    TapeSorter(const std::string& config_path, const std::string& input_file, const std::string& output_file, int max_size):
        config_path(config_path), M(max_size)
    {
        if (!std::filesystem::exists(input_file))
            throw std::runtime_error("No input file!");
        if (std::filesystem::file_size(input_file) == 0)
            throw std::runtime_error("Empty input file!");
        if (!std::filesystem::exists(config_path))
            throw std::runtime_error("No config file!");

        utils::create_file_if_not_exist(output_file);

        input_tape = std::move(T<N>{config_path, input_file});
        output_tape = std::move(T<N>{config_path, output_file});

        chunk_size = M / sizeof(N);
        loaded_data.reserve(chunk_size);

        chunks_number = std::ceil(static_cast<double>(std::filesystem::file_size(input_file)) / M);

        prepare_tmp_tapes();
    }

    void sort()
    {
        read_chunk();

        if (chunks_number == 1)
        {
            save_chunk_in_tape(output_tape);
            return;
        }

        save_chunk_in_tape(tmp_tapes[0]);

        for (int i : std::ranges::iota_view{1, chunks_number})
        {
            read_chunk();
            if (i == chunks_number - 1) [[unlikely]]
                merge_chunks(tmp_tapes[(i - 1) % 2], output_tape);
            else [[likely]]
                merge_chunks(tmp_tapes[(i - 1) % 2], tmp_tapes[i % 2]);
        }
    }
};