#pragma once

#include "encoding_manager.hpp"

namespace QLogicaeAiseCore
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

        void setup_async(
            const std::function<void(const bool& result)>& callback
        );

        void setup_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback
        );

        bool setup(
            const std::string model_path
        );

        std::future<bool> setup_async(
            const std::string model_path
        );

        void setup_async(
            QLogicaeCore::Result<std::future<void>>& result,
            const std::string model_path
        );

        void setup(
            QLogicaeCore::Result<void>& result,
            const std::string model_path
        );

        void setup_async(
            const std::function<void(const bool& result)>& callback,
            const std::string model_path
        );

        void setup_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback,
            const std::string model_path
        );

        bool terminate();

        std::future<bool> terminate_async();

        void terminate_async(
            QLogicaeCore::Result<std::future<void>>& result
        );

        void terminate(
            QLogicaeCore::Result<void>& result
        );

        void terminate_async(
            const std::function<void(const bool& result)>& callback
        );

        void terminate_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback
        );

        double predict(
            const std::string& input
        );

        void predict(
            QLogicaeCore::Result<double>& result,
            const std::string& input
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

        std::mutex _mutex;

        Ort::AllocatorWithDefaultOptions _ort_allocator;

        Ort::Env _env;
        
        Ort::SessionOptions _session_options;
        
        Ort::RunOptions _run_options;
        
        std::unique_ptr<Ort::Session> _session;

        Ort::MemoryInfo _memory_info;

        std::string _input_name;
        
        std::string _output_name;

        const char* _input_names[1];

        const char* _output_names[1];

        std::vector<float> _boc_collection;

        std::size_t _boc_collection_size;

        std::array<int64_t, 2> _shape;

        Ort::Value _tensor { nullptr };
    };
}
