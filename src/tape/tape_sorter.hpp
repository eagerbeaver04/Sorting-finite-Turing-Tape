#pragma once

#include <filesystem>
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
    
};