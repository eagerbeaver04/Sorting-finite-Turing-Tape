#include "gtest/gtest.h"
#include <random>
#include <numeric>
#include "../src/tape/tape_sorter.hpp"
#include "../src/tape/k_way_tape_sorter.hpp"

template <std::integral T>
class Data
{
public:
    std::vector<std::string> configs;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<std::vector<T>> data;
    const int number = 10;
    const int mul = 20;
    void file_init()
    {
        for (int i = 0; i < number; ++i)
        {
            configs.push_back("tests/configs/config" + std::to_string(i) + ".txt");
            inputs.push_back("tests/inputs/input" + std::to_string(i) + ".bin");
            outputs.push_back("tests/outputs/output" + std::to_string(i) + ".bin");
        }
    }
    void config_preparation()
    {
        for (int i = 0; i < number; ++i)
        {
            utils::create_file_if_not_exist(configs[i]);
            std::ofstream outFile(configs[i]);
            if (!outFile)
                std::cerr << "Ошибка при открытии файла!" << std::endl;
            outFile << 0 << std::endl;
            outFile << 0 << std::endl;
            outFile << 0 << std::endl;
            outFile << 0 << std::endl;
            outFile.close();
            utils::create_file_if_not_exist(inputs[i]);
            utils::create_file_if_not_exist(outputs[i]);
        }
    }
    void data_preparation()
    {
        std::random_device rd;
        std::mt19937 gen{rd()};

        for (int i = 0; i < number; ++i)
        {
            std::vector<T> cur(mul * i);
            std::iota(cur.begin(), cur.end(), 1);

            std::ranges::shuffle(cur, gen);
            
            data.emplace_back(cur);
        }
        for (int i = 0; i < number; ++i)
        {
            FileTape<T> tape(configs[i], inputs[i]);
            for (const auto &val : data[i])
                tape.write(val);
            ASSERT_EQ(std::filesystem::file_size(inputs[i]), sizeof(T) * mul * i);
        }
    }

    void read_inputs(int start_number = 0)
    {
        for (int i = start_number; i < number; ++i)
        {
            ASSERT_EQ(std::filesystem::file_size(inputs[i]), sizeof(T) * data[i].size());
            FileTape<T> tape(configs[i], inputs[i]);
            std::vector<T> new_data;
            std::optional<T> val;
            while(!tape.is_eof())
            {   
                val = tape.read();
                if(!val)
                    break;      
                new_data.push_back(val.value());
            }
            ASSERT_EQ(new_data.size(), data[i].size());
            EXPECT_EQ(new_data, data[i]);
        }
    }

    void read_outputs(int start_number = 0)
    {
        for (int i = start_number; i < number; ++i)
        {
            ASSERT_EQ(std::filesystem::file_size(outputs[i]), std::filesystem::file_size(inputs[i]));
            FileTape<T> tape(configs[i], outputs[i]);
            std::vector<T> new_data;
            std::optional<T> val;

            while ((val = tape.read()))
                new_data.push_back(val.value());

            std::vector<T> sorted_data = data[i];
            std::ranges::sort(sorted_data);

            ASSERT_FALSE(sorted_data == data[i]);
            ASSERT_EQ(new_data.size(), data[i].size());
            EXPECT_EQ(sorted_data, new_data);
        }
    }
};

using TestTypes = ::testing::Types<char, unsigned char, short, unsigned short, int, 
    unsigned int, long long, unsigned long long>; 
TYPED_TEST_SUITE(FileTapeSorterTest, TestTypes);

template <std::integral T>
class FileTapeSorterTest : public ::testing::Test
{
protected:
    Data<T> data;

    void SetUp() override
    {
        EXPECT_NO_THROW(data.file_init());
        EXPECT_NO_THROW(data.config_preparation());
    }
};

TYPED_TEST(FileTapeSorterTest, FileTapeCreation)
{
    EXPECT_NO_THROW(this->data.data_preparation());
}

TYPED_TEST(FileTapeSorterTest, FileTapeEmptyFile)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_NO_THROW(this->data.read_inputs());
}

TYPED_TEST(FileTapeSorterTest, FileTapeNonEmptyFiles)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_NO_THROW(this->data.read_inputs(1));
}

template<std::integral T>
void test_sorter(Data<T>& data, int start_pos)
{
    for (int i = start_pos; i < data.number; ++i)
    {
        std::random_device rd;                     
        std::mt19937 gen{rd()};              
        std::uniform_int_distribution<int> dist(sizeof(T), 100 * sizeof(T));

        int random_number = dist(gen);

        TapeSorter<FileTape, T> sorter(data.configs[i], data.inputs[i],
                data.outputs[i], random_number);
        sorter.sort();
    }
}

template <std::integral T>
void bad_test_sorter(Data<T> &data, int start_pos)
{
    for (int i = start_pos; i < data.number; ++i)
    {
        TapeSorter<FileTape, T> sorter(data.configs[i], data.inputs[i],
                data.outputs[i], sizeof(T) / 2);
        sorter.sort();
    }
}

TYPED_TEST(FileTapeSorterTest, TestSortEmptyInputFile)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_ANY_THROW(test_sorter(this->data, 0));
}

TYPED_TEST(FileTapeSorterTest, TestSortSmallMemoryBound)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_ANY_THROW(bad_test_sorter(this->data, 1));
}

TYPED_TEST(FileTapeSorterTest, TestSortNonEmptyInputFile)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_NO_THROW(test_sorter(this->data, 1));
    EXPECT_NO_THROW(this->data.read_outputs(1));
}

template <std::integral T>
void k_way_test_sorter(Data<T> &data, int start_pos)
{
    for (int i = start_pos; i < data.number; ++i)
    {
        std::random_device rd;
        std::mt19937 gen{rd()};
        std::uniform_int_distribution<int> dist(sizeof(T) * 3, sizeof(T) * 3 + 1);

        int random_number = dist(gen);

        KWayTapeSorter<FileTape, T> k_sorter(data.configs[i], data.inputs[i],
            data.outputs[i], random_number, 4);
        k_sorter.sort();
    }
}

TYPED_TEST(FileTapeSorterTest, KWayTestSortEmptyInputFile)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_ANY_THROW(k_way_test_sorter(this->data, 0));
}

TYPED_TEST(FileTapeSorterTest, KWayTestSortNonEmptyInputFile)
{
    EXPECT_NO_THROW(this->data.data_preparation());
    EXPECT_NO_THROW(k_way_test_sorter(this->data, 1));
    EXPECT_NO_THROW(this->data.read_outputs(1));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}