#pragma once

#include "aise_api_configurations.hpp"
#include "aise_api_file_evaluation_results.hpp"
#include "aise_api_file_line_evaluation_results.hpp"
#include "aise_api_file_system_evaluation_results.hpp"
#include "aise_api_file_system_evaluation_configurations.hpp"

namespace QLogicaeAiseCore
{
    class AiseApi
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

        bool setup(
            const AiseApiConfigurations& configurations
        );

        std::future<bool> setup_async(
            const AiseApiConfigurations& configurations
        );

        void setup_async(
            QLogicaeCore::Result<std::future<void>>& result,
            const AiseApiConfigurations& configurations
        );

        void setup(
            QLogicaeCore::Result<void>& result,
            const AiseApiConfigurations& configurations
        );

        std::future<bool> setup_async(
            const std::function<void(const bool& result)>& callback,
            const AiseApiConfigurations& configurations
        );

        void setup_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback,
            const AiseApiConfigurations& configurations
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

        AiseApiFileSystemEvaluationResults evaluate(
            const AiseApiFileSystemEvaluationConfigurations& configurations
        );

        void evaluate(
            QLogicaeCore::Result<AiseApiFileSystemEvaluationResults>& result,
            const AiseApiFileSystemEvaluationConfigurations& configurations
        );

        std::future<AiseApiFileSystemEvaluationResults> evaluate_async(
            const AiseApiFileSystemEvaluationConfigurations& configurations
        );

        void evaluate_async(
            QLogicaeCore::Result<std::future<AiseApiFileSystemEvaluationResults>>& result,
            const AiseApiFileSystemEvaluationConfigurations& configurations
        );

        void evaluate_async(
            const std::function<void(const AiseApiFileSystemEvaluationResults& result)>& callback,
            const AiseApiFileSystemEvaluationConfigurations& configurations
        );

        void evaluate_async(
            const std::function<void(QLogicaeCore::Result<AiseApiFileSystemEvaluationResults>& result)>& callback,
            const AiseApiFileSystemEvaluationConfigurations& configurations
        );

        static AiseApi& get_instance();

        static void get_instance(
            QLogicaeCore::Result<AiseApi*>& result
        );

    protected:
        AiseApi();

        ~AiseApi();

        AiseApi(
            const AiseApi& instance
        ) = default;

        AiseApi(
            AiseApi&& instance
        ) noexcept = delete;

        AiseApi& operator = (
            AiseApi&& instance
        ) = delete;

        AiseApi& operator = (
            const AiseApi& instance
        ) = delete;

        std::mutex _mutex;

        AiseApiConfigurations _configurations;
    };

    inline static AiseApi& AISE_API =
        AiseApi::get_instance();
}
