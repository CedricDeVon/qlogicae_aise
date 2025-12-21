#include "pch.hpp"

#include "qlogicae_aise_core/includes/encoding_manager.hpp"
#include "qlogicae_aise_core/includes/neural_network_model.hpp"

#include <gtest/gtest.h>

#include <future>
#include <thread>

namespace QLogicaeAiseCoreTest
{
    class NeuralNetworkModelTest : public ::testing::Test
    {
    protected:
        QLogicaeAiseCore::EncodingManager& encoding_manager =
            QLogicaeAiseCore::EncodingManager::get_instance();

        QLogicaeAiseCore::NeuralNetworkModel& model =
            QLogicaeAiseCore::NeuralNetworkModel::get_instance();
        
        const std::string valid_model_path = "model.onnx";
        
        const std::string invalid_model_path = "invalid_model.onnx";

        void SetUp() override
        {
            bool result = encoding_manager.load_vocabulary_file_content(
                "vocabulary.json",
                97
            );
        }

        void TearDown() override
        {
            
        }
    };

    TEST_F(NeuralNetworkModelTest, Should_SetupSynchronouslySuccessfully_When_ValidModelPath)
    {
        bool result = model.setup(valid_model_path);
        ASSERT_TRUE(result);
    }

    TEST_F(NeuralNetworkModelTest, Should_FailSetupSynchronously_When_InvalidModelPath)
    {
        bool result = model.setup(invalid_model_path);
        ASSERT_FALSE(result);
    }

    TEST_F(NeuralNetworkModelTest, Should_SetupAsynchronouslySuccessfully_When_ValidModelPath)
    {
        auto future_result = model.setup_async(valid_model_path);
        ASSERT_TRUE(future_result.get());
    }

    TEST_F(NeuralNetworkModelTest, Should_SetupAsynchronouslyWithCallbackSuccessfully_When_ValidModelPath)
    {
        std::promise<bool> promise;
        auto future = promise.get_future();
        model.setup_async(
            [&promise](const bool& result)
            {
                promise.set_value(result);
            },
            valid_model_path
        );
        ASSERT_TRUE(future.get());
    }

    TEST_F(NeuralNetworkModelTest, Should_TerminateSynchronouslySuccessfully)
    {
        bool result = model.terminate();
        ASSERT_TRUE(result);
    }

    TEST_F(NeuralNetworkModelTest, Should_TerminateAsynchronouslySuccessfully)
    {
        auto future_result = model.terminate_async();
        ASSERT_TRUE(future_result.get());
    }

    TEST_F(NeuralNetworkModelTest, Should_TerminateAsynchronouslyWithCallbackSuccessfully)
    {
        std::promise<bool> promise;
        auto future = promise.get_future();
        model.terminate_async(
            [&promise](const bool& result)
            {
                promise.set_value(result);
            }
        );
        ASSERT_TRUE(future.get());
    }

    class NeuralNetworkModelPredictTest : public NeuralNetworkModelTest,
        public ::testing::WithParamInterface<std::string>
    {
    };

    INSTANTIATE_TEST_CASE_P(
        PredictInputs,
        NeuralNetworkModelPredictTest,
        ::testing::Values(
            "Hello world",
            "Test input string",
            "",
            "1234567890!@#$%^&*()"
        )
    );

    TEST_P(NeuralNetworkModelPredictTest, Should_PredictSuccessfully_When_InputString)
    {
        std::string input = GetParam();
        double result = model.predict(input);
        ASSERT_GE(result, 0.0);
        ASSERT_LE(result, 1.0);
    }

    TEST_F(NeuralNetworkModelTest, Should_PredictSynchronouslyAndConsistentlyUnderMultithreadedStress)
    {
        const size_t thread_count = 16;
        const size_t iterations_per_thread = 100;
        std::vector<std::thread> threads;
        std::atomic<bool> failed{ false };

        for (size_t i = 0; i < thread_count; ++i)
        {
            threads.emplace_back([&]()
                {
                    for (size_t j = 0; j < iterations_per_thread; ++j)
                    {
                        double result = model.predict("Stress test input string");
                        if (result < 0.0 || result > 1.0)
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

    TEST_F(NeuralNetworkModelTest, Should_PredictAsynchronouslySuccessfully)
    {
        std::promise<double> promise;
        auto future = promise.get_future();
        std::thread([&]()
            {
                double result = model.predict("Async prediction input");
                promise.set_value(result);
            }).detach();

        double prediction = future.get();
        ASSERT_GE(prediction, 0.0);
        ASSERT_LE(prediction, 1.0);
    }

    TEST_F(NeuralNetworkModelTest, Should_HandleEmptyInputStringPredict)
    {
        double prediction = model.predict("");
        ASSERT_GE(prediction, 0.0);
        ASSERT_LE(prediction, 1.0);
    }

    TEST_F(NeuralNetworkModelTest, Should_HandleVeryLargeInputStringPredict)
    {
        std::string input(100000, 'A');
        double prediction = model.predict(input);
        ASSERT_GE(prediction, 0.0);
        ASSERT_LE(prediction, 1.0);
    }

    TEST_F(NeuralNetworkModelTest, Should_HandleNonASCIIInputStringPredict)
    {
        std::string input(reinterpret_cast<const char*>(u8"非ASCII文字列テスト"));
        double prediction = model.predict(input);
        ASSERT_GE(prediction, 0.0);
        ASSERT_LE(prediction, 1.0);
    }

    TEST_F(NeuralNetworkModelTest, Should_HandleEmptyModelPathSetup)
    {
        ASSERT_FALSE(model.setup(""));
    }

    TEST_F(NeuralNetworkModelTest, Should_HandleInvalidModelPathSetup)
    {
        ASSERT_FALSE(model.setup("invalid_model_path.onnx"));
    }

    TEST_F(NeuralNetworkModelTest, Should_AsyncSetupWithPromiseComplete)
    {
        std::promise<bool> promise;
        auto future = promise.get_future();
        std::thread([&]()
            {
                promise.set_value(model.setup());
            }).detach();
        ASSERT_TRUE(future.get());
    }

    TEST_F(NeuralNetworkModelTest, Should_AsyncTerminateWithPromiseComplete)
    {
        std::promise<bool> promise;
        auto future = promise.get_future();
        std::thread([&]()
            {
                promise.set_value(model.terminate());
            }).detach();
        ASSERT_TRUE(future.get());
    }

    TEST_F(NeuralNetworkModelTest, Should_PredictAfterTerminateWithoutCrash)
    {
        model.terminate();
        double prediction = model.predict("Test after terminate");
        ASSERT_GE(prediction, 0.0);
        ASSERT_LE(prediction, 1.0);
    }

}