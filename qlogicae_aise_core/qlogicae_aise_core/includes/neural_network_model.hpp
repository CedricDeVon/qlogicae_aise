#pragma once

namespace QLogicaeAiseConsole
{
    class NeuralNetworkModel
    {
    public:
        bool setup();

        std::future<bool> setup_async();

        void setup_async(
            QLogicaeCore::Result<std::future<void>>& result
        );

        void setup(
            QLogicaeCore::Result<void>& result
        );

        std::future<bool> setup_async(
            const std::function<void(const bool& result)>& callback
        );

        void setup_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback
        );

        bool terminate();

        std::future<bool> terminate_async();

        void terminate_async(
            QLogicaeCore::Result<std::future<void>>& result
        );

        void terminate(
            QLogicaeCore::Result<void>& result
        );

        std::future<bool> terminate_async(
            const std::function<void(const bool& result)>& callback
        );

        void terminate_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback
        );

        double predict(
            const std::vector<float>& input
        );

        void predict(
            QLogicaeCore::Result<double>& result,
            const std::vector<float>& input
        );

        static NeuralNetworkModel& get_instance();

        static void get_instance(
            QLogicaeCore::Result<NeuralNetworkModel*>& result
        );

    protected:
        NeuralNetworkModel();

        ~NeuralNetworkModel();

        NeuralNetworkModel(
            const NeuralNetworkModel& instance
        ) = default;

        NeuralNetworkModel(
            NeuralNetworkModel&& instance
        ) noexcept = delete;

        NeuralNetworkModel& operator = (
            NeuralNetworkModel&& instance
            ) = delete;

        NeuralNetworkModel& operator = (
            const NeuralNetworkModel& instance
            ) = delete;

        float* raw;
        
        std::array<int64_t, 2> shape;
        
        std::size_t total;
        
        std::vector<Ort::Value> outputs;
        
        Ort::AllocatorWithDefaultOptions alloc;

        Ort::Env env_;
        
        Ort::SessionOptions session_options_;
        
        Ort::RunOptions run_options_;
        
        std::unique_ptr<Ort::Session> session_;

        Ort::MemoryInfo memory_info_;

        std::string input_name_;
        
        std::string output_name_;

        const char* input_names_[1];

        const char* output_names_[1];
    };

    inline static NeuralNetworkModel& NEURAL_NETWORK_MODEL =
        NeuralNetworkModel::get_instance();
}
