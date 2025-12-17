#pragma once

#include "aise_api_file_evaluation_results.hpp"

namespace QLogicaeAiseCore
{
	struct AiseApiFileSystemEvaluationResults
	{
		std::unordered_map<std::string, AiseApiFileEvaluationResults> file_evaluation_results;

		std::unordered_map<std::string, AiseApiFileEvaluationResults> positive_file_evaluation_results;

		std::size_t total_line_count = 0;

		std::size_t total_positive_prediction_count = 0;

		double timestamp_start = QLogicaeCore::TIME.now();

		double timestamp_end = 0.0;
	};
}
