set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")
add_definitions(-DTEST_DATA_DIR=\"${TEST_DATA_DIR}\")

find_package(GTest REQUIRED)

add_executable(lirs_test
			   lirs_test.cpp
			   main.cpp)

target_compile_options(lirs_test PRIVATE -O2)

target_link_libraries(lirs_test
					  GTest::GTest
					  GTest::Main)

target_include_directories(lirs_test PRIVATE
	                       ${CMAKE_SOURCE_DIR}/unit_test/funcs
	                       ${CMAKE_SOURCE_DIR}/lirs
)

enable_testing()
add_test(NAME LirsTest COMMAND lirs_test)