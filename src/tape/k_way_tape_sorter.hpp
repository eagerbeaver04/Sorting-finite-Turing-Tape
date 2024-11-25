#pragma once

#include <filesystem>
#include <array>
#include <vector>
#include <ranges>
#include <algorithm>
#include <exception>
#include <queue>
#include "tape_sorter.hpp"

template <template <typename> typename T, typename N>
    requires(Tape<T, N>)
class KWayTapeSorter
{
private:
    size_t M;
    size_t k;
    size_t chunk_size;
    size_t chunks_number;
    T<N> input_tape;
    T<N> output_tape;
    std::vector<T<N>> tmp_tapes{};
    std::vector<N> loaded_data{};
    std::priority_queue<
        std::pair<N, int>,
        std::vector<std::pair<N, int>>,
        std::greater<std::pair<N, int>>>
        queue;

    void prepare_tmp_tapes(const std::string &config_path)
    {
        tmp_tapes.resize(k+2);
        for (size_t i = 0; i < k+2; ++i)
        {
            std::string file_path = "tmp/tmp_line_" + std::to_string(i) + ".bin";
            utils::create_file_if_not_exist(file_path);
            tmp_tapes[i] = std::move(T<N>(config_path, file_path));
        }
    }

    void merge_chunks(std::vector<std::reference_wrapper<T<N>>> &input_tapes, T<N> &output_tmp_tape)
    {
        queue = {};
        std::optional<N> val;
        output_tmp_tape.rewind();
        for (size_t i = 0; i < input_tapes.size(); ++i)
        {
            input_tapes[i].get().rewind();
            val = input_tapes[i].get().read();
            if (val)
                queue.emplace(val.value(), i);
        }
        size_t count = 0;
        std::pair<N, int> cur;
        while (count < input_tapes.size())
        {
            if(queue.empty())
                break;
            cur = queue.top();
            queue.pop();
            output_tmp_tape.write(cur.first);
            val = input_tapes[cur.second].get().read();
            if (val)
                queue.emplace(val.value(), cur.second);
            else
                ++count;
        }
    }

    bool read_chunk()
    {
        loaded_data.clear();
        std::ranges::for_each(
            std::ranges::views::iota(0) | std::ranges::views::take(chunk_size),
            [&](auto)
            {
                std::optional<N> opt_val = input_tape.read();
                if (!opt_val)
                    return;
                loaded_data.push_back(opt_val.value());
            });
        utils::merge_sort(loaded_data.begin(), loaded_data.end());
        if(loaded_data.size() != chunk_size)
            return false;
        return true;
    }

    void save_chunk_in_tape(T<N> &tape)
    {
        tape.rewind();
        for (auto &&el : loaded_data)
            tape.write(el);
    }

public:
    KWayTapeSorter(const std::string &config_path, const std::string &input_file, const std::string &output_file, size_t max_size, size_t number_of_tapes) : M(max_size), k(number_of_tapes)
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

        size_t total_elements = std::filesystem::file_size(input_file) / sizeof(N);
        chunks_number = (total_elements + chunk_size - 1) / chunk_size;

        // std::cout << "file size (bytes)= " << std::filesystem::file_size(input_file)
        //     <<  ", number of elements = " << total_elements
        //     <<", chunks number = " << chunks_number
        //     << "\n\telements in chunk = " << chunk_size
        //     << ", size of chunk (bytes)= " << chunk_size * sizeof(N)
        //     << ", memory limit (bytes)= " << M << std::endl;
        prepare_tmp_tapes(config_path);
    }

    void sort()
    {
        size_t current_chunks = 0;
        bool alternate = false;
        std::vector<std::reference_wrapper<T<N>>> merge_inputs{};
        merge_inputs.reserve(k+1);
        while (true)
        {
            if (!read_chunk())
            {
                std::optional<N> val;

                for (auto &&el : loaded_data)
                    output_tape.write(el);
                output_tape.rewind();
                merge_inputs.clear();
                merge_inputs.push_back(output_tape);

                if (alternate)
                {
                    merge_inputs.push_back(tmp_tapes[k + 1]);
                    merge_chunks(merge_inputs, tmp_tapes[0]);
                }
                else
                {
                    merge_inputs.push_back(tmp_tapes[0]);
                    merge_chunks(merge_inputs, tmp_tapes[k + 1]);
                }
                break;
            }

            save_chunk_in_tape(tmp_tapes[(current_chunks + 1) % (k + 1)]);
            ++current_chunks;
        
            if (current_chunks == k)
            {
                merge_inputs = std::vector<std::reference_wrapper<T<N>>>(
                    tmp_tapes.begin() + static_cast<int>(alternate),
                    tmp_tapes.begin() + static_cast<int>(alternate) + k + 1);
                T<N> &target_tape = alternate ? tmp_tapes[0] : tmp_tapes[k+1];
                merge_chunks(merge_inputs, target_tape);

                for (auto &tape : tmp_tapes)
                    tape.rewind();

                alternate = !alternate;
                current_chunks = 0;
            }
        }
        if (current_chunks == 0)
        {
            merge_inputs.clear();
            T<N> &target_tape = alternate ? tmp_tapes[0] : tmp_tapes[k + 1];
            merge_inputs.push_back(target_tape);
            merge_chunks(merge_inputs, output_tape);
            return;
        }
        if (current_chunks > 0)
        {
            merge_inputs  = std::vector<std::reference_wrapper<T<N>>>(tmp_tapes.begin() + 1, tmp_tapes.begin() + current_chunks + 1);
            T<N> &target_tape = alternate ? tmp_tapes[0] : tmp_tapes[k + 1];
            merge_inputs.push_back(target_tape);

            merge_chunks(merge_inputs, output_tape);
            return;
        }
    }
};