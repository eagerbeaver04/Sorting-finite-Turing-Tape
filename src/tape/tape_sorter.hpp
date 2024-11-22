#pragma once

#include <filesystem>
#include <array>
#include <vector>
#include <ranges>
#include <cmath>
#include <algorithm>
#include "file_tape.hpp"

namespace
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
    std::array<T<N>, 2> tmp_tapes;
    std::vector<N> loaded_data;

    void create_file_if_not_exist(const std::string& file_path)
    {
        if (!std::filesystem::exists(file_path))
        {
            std::ofstream outfile(file_path);
            outfile.close();
        }
    }
    void prepare_tmp_tapes()
    {
        for(int i : std::ranges::iota_view(0, tmp_tapes.size()))
        {
            std::string file_path = "tmp/tmp_line_" + std::to_string(i) + ".bin";
            create_file_if_not_exist(file_path);
            tmp_tapes[i] = std::move(T<N>(config_path, file_path));
        }
    }
public:
    void sort();
    TapeSorter(const std::string& config_path, const std::string& input_file, const std::string& output_file, size_t M):
        config_path(config_path), M(M)
    {
        if (!std::filesystem::exists(input_file))
            throw std::runtime_error("No input file!");
        if (!std::filesystem::exists(config_path))
            throw std::runtime_error("No config file!");

        create_file_if_not_exist(output_file);
        input_tape(config_path, input_file);
        output_tape(config_path, output_file);

        chunk_size = M / sizeof(N);
        loaded_data.reserve(chunk_size);

        chunks_number = std::ceil(std::filesystem::file_size(input_file) / M);

        prepare_tmp_tapes();
    }
};