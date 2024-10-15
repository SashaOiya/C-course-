#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "lirs.hpp"
#include "func.hpp"

TEST(test1, cache2_data13)
{
	test_funcs::run_test("data", false);
}

TEST(test2, cache10_data101)
{
	test_funcs::run_test("data2", false);
}

TEST(test3, cache2_data5)
{
	test_funcs::run_test("data4", false);
}
