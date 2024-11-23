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
        else
            std::ofstream file(file_path, std::ios::trunc);
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
    size_t M;
    size_t chunk_size;
    size_t chunks_number;
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
        size_t pos = 0;
        std::optional<N> val;
        bool f = false;
        while(pos < loaded_data.size())
        {
            if(!f)
            {
                val = input_tmp_tape.read();
                if (!val)
                    break;
                f = true;
            }
            if(val.value() <= loaded_data[pos])
            {
                output_tmp_tape.write(val.value());
                f = false;
            }
            else
            {
                output_tmp_tape.write(loaded_data[pos]);
                ++pos;
            }
        }
        for (int i : std::ranges::iota_view{pos, loaded_data.size()})
        {
            output_tmp_tape.write(loaded_data[i]);
        }
        while (val)
        {
            output_tmp_tape.write(val.value());
            val = input_tmp_tape.read();
        }
    }

    void read_chunk()
    {
        loaded_data.clear();
        std::ranges::for_each(
            std::ranges::views::iota(0) | std::ranges::views::take(chunk_size),
            [&](auto)
            {
                std::optional<N> opt_val = input_tape.read();
                if (!opt_val)
                    return;
                loaded_data.push_back(*opt_val);
            });
        utils::merge_sort(loaded_data.begin(), loaded_data.end());
    }

    void save_chunk_in_tape(T<N>& tape)
    {
        tape.rewind();
        for(auto&& el : loaded_data)
            tape.write(el);
    }
public:
    TapeSorter(const std::string& config_path, const std::string& input_file, const std::string& output_file, size_t max_size):
        config_path(config_path), M(max_size)
    {
        if (M < sizeof(N))
            throw std::runtime_error("Too small chunk size!");
        if (!std::filesystem::exists(input_file))
            throw std::runtime_error("No input file! " + input_file);
        if (std::filesystem::file_size(input_file) == 0)
            throw std::runtime_error("Empty input file! " + input_file);
        if (!std::filesystem::exists(config_path))
            throw std::runtime_error("No config file! " + config_path);

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
        for (size_t i = 1; i < chunk_size; ++i)
        {
            read_chunk();
            if (i == chunks_number - 1) [[unlikely]]
                merge_chunks(tmp_tapes[(i - 1) % 2], output_tape);
            else [[likely]]
                merge_chunks(tmp_tapes[(i - 1) % 2], tmp_tapes[i % 2]);
        }
    }
};