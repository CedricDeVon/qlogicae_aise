#include "pch.hpp"

#include "main.hpp"

namespace QLogicaeAiseCoreTest
{
	TEST(TestCaseName, TestName)
	{
		QLogicaeCore::CLI_IO.print_with_new_line("QLogicaeAiseCoreTest - Confirmed!");

		EXPECT_EQ(1, 1);
		EXPECT_TRUE(true);
	}
}
