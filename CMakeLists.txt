cmake_minimum_required(VERSION 3.11)

project(cache)

find_package(GTest REQUIRED)
enable_testing()

# set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g -O3
#                            -Wmissing-declarations -Wcast-align 
#                            -Wunnused -Wshadow --Wsign-conversion
#                            -fsanitize=address -fsanitize=undefined")

add_subdirectory(./lirs)
add_subdirectory(./ideal)

add_subdirectory(./unit_test/lirs)
add_subdirectory(./unit_test/ideal)
