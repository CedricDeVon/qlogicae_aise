#include "pch.hpp"

#include "qlogicae_aise_core/includes/encoding_manager.hpp"

#include <gtest/gtest.h>

#include <future>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>

namespace QLogicaeAiseCoreTest
{
    class EncodingManagerTest : public testing::Test
    {
    protected:
        QLogicaeAiseCore::EncodingManager& encoding_manager =
            QLogicaeAiseCore::ENCODING_MANAGER;
        const std::string valid_vocabulary_file_path = "vocabulary.json";

        void SetUp() override
        {
            std::ofstream file(valid_vocabulary_file_path);
            file << R"(["a","b","c","d","<UNK>"])";
            file.close();
        }

        void TearDown() override
        {
            std::remove(valid_vocabulary_file_path.c_str());
        }
    };

    class EncodingManagerParameterizedTest : public EncodingManagerTest,
        public testing::WithParamInterface<std::string>
    {
    };

    TEST_F(EncodingManagerTest, Should_LoadVocabulary_Success_When_ValidFile)
    {
        bool result = encoding_manager.load_vocabulary_file_content(
            valid_vocabulary_file_path,
            5
        );
        ASSERT_TRUE(result);
    }

    TEST_F(EncodingManagerTest, Should_ReturnFalse_When_FileNotFound)
    {
        bool result = encoding_manager.load_vocabulary_file_content(
            "non_existent_file.json",
            5
        );
        ASSERT_FALSE(result);
    }

    TEST_F(EncodingManagerTest, Should_NormalizeBocVector_When_TextProvided)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        auto boc_vector = encoding_manager.from_string_to_boc("abcd");
        ASSERT_EQ(boc_vector.size(), 5);
        float sum_of_squares = 0.0f;
        for (auto value : boc_vector)
        {
            sum_of_squares += value * value;
        }
        ASSERT_NEAR(sum_of_squares, 1.0f, 0.0001f);
    }

    TEST_F(EncodingManagerTest, Should_HandleEmptyText_When_FromStringToBocCalled)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        auto boc_vector = encoding_manager.from_string_to_boc("");
        for (auto value : boc_vector)
        {
            ASSERT_EQ(value, 0.0f);
        }
    }

    TEST_F(EncodingManagerTest, Should_HandleSingleCharacter_When_FromStringToBocCalled)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        auto boc_vector = encoding_manager.from_string_to_boc("a");
        ASSERT_EQ(boc_vector[0] > 0.0f, true);
    }

    TEST_F(EncodingManagerTest, Should_SetupAsync_CompleteSuccessfully_When_Called)
    {
        auto future_result = encoding_manager.setup_async();
        ASSERT_TRUE(future_result.get());
    }

    TEST_F(EncodingManagerTest, Should_TerminateAsync_CompleteSuccessfully_When_Called)
    {
        auto future_result = encoding_manager.terminate_async();
        ASSERT_TRUE(future_result.get());
    }

    TEST_F(EncodingManagerTest, Should_InvokeCallbackWithCorrectResult_When_AsyncCallbackUsed)
    {
        bool callback_called = false;
        encoding_manager.setup_async([&callback_called](const bool& result)
            {
                callback_called = result;
            });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ASSERT_TRUE(callback_called);
    }

    TEST_F(EncodingManagerTest, Should_ReturnFalse_When_VocabularyFileInvalid)
    {
        std::ofstream file("invalid.json");
        file << "invalid_json_content";
        file.close();
        bool result = encoding_manager.load_vocabulary_file_content("invalid.json", 5);
        ASSERT_FALSE(result);
        std::remove("invalid.json");
    }

    TEST_F(EncodingManagerTest, Should_HandleConcurrentFromStringToBoc_Correctly)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        std::vector<std::future<std::vector<float>>> futures;
        for (int i = 0; i < 10; ++i)
        {
            futures.push_back(std::async(std::launch::async, [&]()
                {
                    return encoding_manager.from_string_to_boc("abcd");
                }));
        }
        for (auto& future : futures)
        {
            auto boc_vector = future.get();
            ASSERT_EQ(boc_vector.size(), 5);
        }
    }

    TEST_F(EncodingManagerTest, Should_StressLoadVocabularyMultipleThreads)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        for (int i = 0; i < 20; ++i)
        {
            threads.emplace_back([&]()
                {
                    encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
                });
        }
        for (auto& thread : threads)
        {
            thread.join();
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        ASSERT_LE(duration.count(), 2.0);
    }

    TEST_P(EncodingManagerParameterizedTest, Should_HandleVariousTextInputs_When_FromStringToBocCalled)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        auto boc_vector = encoding_manager.from_string_to_boc(GetParam());
        ASSERT_EQ(boc_vector.size(), 5);
    }

    INSTANTIATE_TEST_CASE_P(
        TextInputVariations,
        EncodingManagerParameterizedTest,
        testing::Values(
            "",
            "a",
            "abcd",
            "abcde",
            "aaaaaaaaaaaaaaaaaaaa"
        )
    );

    TEST_F(EncodingManagerTest, Should_HandleInvalidJson_ExceptionSafety)
    {
        std::ofstream file("corrupt.json");
        file << "{ invalid json ";
        file.close();

        bool result = false;
        try
        {
            result = encoding_manager.load_vocabulary_file_content("corrupt.json", 5);
        }
        catch (...)
        {
            result = false;
        }

        ASSERT_FALSE(result);
        std::remove("corrupt.json");
    }

    TEST_F(EncodingManagerTest, Should_HandleUnicodeText_When_FromStringToBocCalled)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        auto boc_vector = encoding_manager.from_string_to_boc("こんにちは世界🌏");
        ASSERT_EQ(boc_vector.size(), 5);
    }

    TEST_F(EncodingManagerTest, Should_HandleVeryLongInput_When_FromStringToBocCalled)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        std::string long_text(10000, 'a');
        auto boc_vector = encoding_manager.from_string_to_boc(long_text);
        ASSERT_EQ(boc_vector.size(), 5);
    }

    TEST_F(EncodingManagerTest, Should_StressFromStringToBoc_Multithreaded)
    {
        encoding_manager.load_vocabulary_file_content(valid_vocabulary_file_path, 5);
        const size_t thread_count = 16;
        const size_t iterations_per_thread = 1000;
        std::vector<std::thread> threads;
        std::atomic<bool> failed{ false };

        for (size_t i = 0; i < thread_count; ++i)
        {
            threads.emplace_back([&]()
                {
                    for (size_t j = 0; j < iterations_per_thread; ++j)
                    {
                        auto boc_vector = encoding_manager.from_string_to_boc("stress test input");
                        if (boc_vector.size() != 5)
                        {
                            failed.store(true, std::memory_order_relaxed);
                        }
                    }
                });
        }

        for (auto& t : threads)
        {
            t.join();
        }

        ASSERT_FALSE(failed.load());
    }

    TEST_F(EncodingManagerTest, Should_PerformAsyncSetupUnderTimeLimit)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto future_result = encoding_manager.setup_async();
        ASSERT_TRUE(future_result.get());
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        ASSERT_LE(duration.count(), 2.0);
    }

    TEST_F(EncodingManagerTest, Should_PerformAsyncTerminateUnderTimeLimit)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto future_result = encoding_manager.terminate_async();
        ASSERT_TRUE(future_result.get());
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        ASSERT_LE(duration.count(), 2.0);
    }
}
