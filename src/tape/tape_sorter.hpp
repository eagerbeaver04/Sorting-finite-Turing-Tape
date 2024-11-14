#pragma once

#include <filesystem>
#include <array>
#include "file_tape.hpp"

template<template<typename> typename T, typename N>
concept Tape = std::is_base_of_v<ITape<N>, T<N>>;

template <template <typename> typename T, typename N>
    requires(Tape<T, N>)
class TapeSorter
{
private:
    T<N> input_tape;
    T<N> output_tape;
    size_t M;
    std::array<T<N>, 2> tmp_tapes;
    void merge_sort();
public:
    void sort();
    TapeSorter(const std::string& input_file, const std::string& output_file, size_t M);
};