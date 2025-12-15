#pragma once

#include "aise_api_file_line_evaluation_results.hpp"

namespace QLogicaeAiseCore
{
	struct AiseApiFileEvaluationResults
	{
		std::vector<AiseApiFileLineEvaluationResults> file_line_evaluation_results;
	};
}
