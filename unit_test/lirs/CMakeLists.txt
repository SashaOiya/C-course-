add_executable(lirs_test
			   lirs_test.cpp)
			   
set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

target_compile_definitions(lirs_test PUBLIC
						   -DTEST_DATA_DIR="${TEST_DATA_DIR}")


set_target_properties(lirs_test PROPERTIES 
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_link_libraries(lirs_test PRIVATE
					  GTest::GTest
					  GTest::gtest_main)

message(STATUS "DIR: ${PROJECT_SOURCE_DIR}")

target_include_directories(lirs_test PRIVATE
					       ${PROJECT_SOURCE_DIR}/lirs
					       ${PROJECT_SOURCE_DIR}/ideal
					       ${PROJECT_SOURCE_DIR}/unit_test/func)

include(GoogleTest)
gtest_discover_tests(lirs_test)
