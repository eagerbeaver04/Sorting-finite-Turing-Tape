#pragma once

#include <vector>
#include "file_tape.hpp"

namespace utils
{
    void create_file_if_not_exist(const std::string &file_path)
    {
        std::ofstream file(file_path, std::ios::trunc);
    }
}

template <template <typename> typename T, typename N>
    requires Tape<T, N>
struct TmpTapeManager : std::bool_constant<false>
{
    static std::vector<T<N>> prepare_tmp_tapes(const std::string &config_path, size_t k)
    {
        throw std::logic_error("Not implemented!");
    }
};

template <typename N>
struct TmpTapeManager<FileTape, N> : std::bool_constant<true>
{
    static std::vector<FileTape<N>> prepare_tmp_tapes(const std::string &config_path, size_t k)
    {
        if (!std::filesystem::is_directory("tmp") || !std::filesystem::exists("tmp"))
            std::filesystem::create_directory("tmp");

        std::vector<FileTape<N>> tmp_tapes;
        tmp_tapes.resize(k);
        for (size_t i = 0; i < k; ++i)
        {
            std::string file_path = "tmp/tmp_line_" + std::to_string(i) + ".bin";
            utils::create_file_if_not_exist(file_path);
            tmp_tapes[i] = std::move(FileTape<N>(config_path, file_path));
        }
        return tmp_tapes;
    }
};
