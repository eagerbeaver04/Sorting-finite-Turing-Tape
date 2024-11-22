#include "gtest/gtest.h"
#include <random>
#include <numeric>
#include "../src/tape/tape_sorter.hpp"


class Data
{
private:
    std::vector<std::string> configs;
    std::vector<std::string> inputs;
    using type = int;
    std::vector<std::vector<type>> data;
    const int number = 5;
public:
    void file_init()
    {
        for (int i = 0; i < number; ++i)
        {
            configs.push_back("tests/configs/config" + std::to_string(i) + ".txt");
            inputs.push_back("tests/inputs/input" + std::to_string(i) + ".bin");
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
            outFile << i << std::endl;
            outFile << i << std::endl;
            outFile << i << std::endl;
            outFile << i << std::endl;
            outFile.close();
            utils::create_file_if_not_exist(inputs[i]);
        }
    }
    void data_preparation()
    {
        std::random_device rd;
        std::mt19937 gen{rd()};

        int mul = 10;

        for (int i = 0; i < number; ++i)
        {
            std::vector<type> cur(mul * i);
            std::iota(cur.begin(), cur.end(), 1);
            std::ranges::shuffle(cur, gen);
            data.emplace_back(cur);
        }
        for (int i = 0; i < number; ++i)
        {
            FileTape<type> tape(configs[i], inputs[i]);
            for (const auto &val : data[i])
                tape.write(val);
            EXPECT_EQ(std::filesystem::file_size(inputs[i]), sizeof(type) * mul * i);
        }
    }

    void read_inputs(int start_number = 0)
    {
        for (int i = start_number; i < number; ++i)
        {
            FileTape<type> tape(configs[i], inputs[i]);
            std::vector<type> new_data;
            type val;
            while(!tape.is_eof())
            {   
                val = tape.read();
                if(tape.is_eof())
                    break;      
                new_data.push_back(val);
            }
            ASSERT_EQ(new_data.size(), data[i].size());
            for(size_t j =0; j < new_data.size(); ++j)
                EXPECT_EQ(new_data[j], data[i][j]);
        }
    }
};

class InputFiles : public ::testing::Test
{
protected:
    Data data;
};

TEST_F(InputFiles, TestCreation)
{
    EXPECT_NO_THROW(data.file_init());
    EXPECT_NO_THROW(data.config_preparation());
    EXPECT_NO_THROW(data.data_preparation());
}

TEST_F(InputFiles, EmptyFile)
{
    EXPECT_NO_THROW(data.file_init());
    EXPECT_NO_THROW(data.data_preparation());
    EXPECT_NO_THROW(data.read_inputs());
}

TEST_F(InputFiles, NonEmptyFiles)
{
    EXPECT_NO_THROW(data.file_init());
    EXPECT_NO_THROW(data.data_preparation());
    EXPECT_NO_THROW(data.read_inputs(1));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}