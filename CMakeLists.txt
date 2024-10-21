cmake_minimum_required(VERSION 3.11)

project(cache)

find_package(GTest REQUIRED)
enable_testing()

add_subdirectory(./lirs)
add_subdirectory(./ideal)

add_subdirectory(./unit_test/lirs)
add_subdirectory(./unit_test/ideal)
