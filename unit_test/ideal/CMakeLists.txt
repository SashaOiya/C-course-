add_executable(ideal_test
			   ideal_test.cpp)
			   
set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

set_target_properties(ideal_test PROPERTIES 
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_compile_definitions(ideal_test PUBLIC
						   -DTEST_DATA_DIR="${TEST_DATA_DIR}")

target_link_libraries(ideal_test PRIVATE
					  GTest::GTest
					  GTest::gtest_main)

message(STATUS "DIR: ${PROJECT_SOURCE_DIR}")

target_include_directories(ideal_test PUBLIC
	                       ${PROJECT_SOURCE_DIR}/ideal
						   ${PROJECT_SOURCE_DIR}/lirs
						   ${PROJECT_SOURCE_DIR}/unit_test/func)

include(GoogleTest)
gtest_discover_tests(ideal_test)
