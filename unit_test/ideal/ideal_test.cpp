#include <gtest/gtest.h>

#include "func.hpp"

TEST(test1, data)
{
	test_funcs::run_test("/test1/data", true);
}

TEST(test2, data2)
{
	test_funcs::run_test("/test2/data2", true);
}

TEST(test4, data4)
{
	test_funcs::run_test("/test4/data4", true);
}
