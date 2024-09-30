cmake_minimum_required(VERSION 3.11)

project(cache)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g -O3
                           -Wmissing-declarations -Wcast-align 
                           -Wunnused -Wshadow --Wsign-conversion
                           -fsanitize=address -fsanitize=undefined")

add_subdirectory(./lirs)
add_subdirectory(./ideal)