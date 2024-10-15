#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "ideal.hpp"
#include "func.hpp"

TEST(test1, cache2_data13)
{
	test_funcs::run_test("data", true);
}

TEST(test2, cache10_data100)
{
	test_funcs::run_test("data2", true);
}

TEST(test3, cache2_data5)
{
	test_funcs::run_test("data4", true);
}

