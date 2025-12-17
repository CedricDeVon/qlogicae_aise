#pragma once

namespace QLogicaeAiseCore
{
	struct AiseApiFileLineEvaluationResults
	{		
		std::string line_text = "";

		std::size_t line_number = 0;

		double line_prediction = 0.0;

		double timestamp_start = QLogicaeCore::TIME.now();

		double timestamp_end = 0.0;
	};
}
