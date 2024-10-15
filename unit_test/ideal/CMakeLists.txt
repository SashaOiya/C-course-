cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")
add_definitions(-DTEST_DATA_DIR=\"${TEST_DATA_DIR}\")

find_package(GTest REQUIRED)

add_executable(ideal_test
			   ideal_test.cpp
			   main.cpp)

target_compile_options(ideal_test PRIVATE -O2)

target_link_libraries(ideal_test
					  GTest::GTest
					  GTest::Main)

target_include_directories(ideal_test PRIVATE
	                       ${CMAKE_SOURCE_DIR}/unit_test/funcs
	                       ${CMAKE_SOURCE_DIR}/ideal
)

enable_testing()
add_test(NAME IdealTest COMMAND ideal_test)