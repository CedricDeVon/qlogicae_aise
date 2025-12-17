#pragma once

#include "aise_api_file_system_evaluation_callback_configurations_results.hpp"

namespace QLogicaeAiseCore
{
	struct AiseApiFileSystemEvaluationConfigurations
	{
		std::string root_folder_path = "";

		std::vector<std::string> expected_file_extensions =
		{
			".hpp",
			".cpp"
		};

		double minimum_positive_prediction = 0.99;

		double maximum_positive_prediction = 1.0;

		std::function<void(const AiseApiFileSystemEvaluationCallbackConfigurationsResults& results)> file_evaluation_callback =
			[](const AiseApiFileSystemEvaluationCallbackConfigurationsResults& results) {};
	};
}
