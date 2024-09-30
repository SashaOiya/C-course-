cmake_minimum_required(VERSION 3.11)

project("ideal_cache")

set(SOURCES
        ideal.cpp
)
set (HEADERS
        ideal.hpp
)

add_executable(ideal ${SOURCES} ${HEADERS})

option(MyOption "MyOption" OFF)

set_target_properties(
        ideal PROPERTIES 
        CXX_STANDART 20
        CXX_STANDARD_REQUIRED ON
)