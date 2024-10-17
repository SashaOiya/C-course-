#include <gtest/gtest.h>

#include "func.hpp"

TEST(test1, data)
{
	test_funcs::run_test("/test1/data", false );
}

TEST(test2, data2)
{
	test_funcs::run_test("/test2/data2", false);
}

TEST(test4, data4)
{
	test_funcs::run_test("/test4/data4", false);
}

