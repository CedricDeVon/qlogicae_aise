#pragma once

#include "aise_api_file_evaluation_results.hpp"

namespace QLogicaeAiseCore
{
	struct AiseApiFileSystemEvaluationResults
	{
		std::unordered_map<std::string, AiseApiFileEvaluationResults> file_evaluation_results;
	};
}
