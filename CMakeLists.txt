cmake_minimum_required(VERSION 3.11)

project(ideal)

set(SOURCES
        ideal.cpp
)
set (HEADERS
        ideal.h
)

add_executable(ideal ${SOURCES} ${HEADERS})

set_target_properties(
        ideal PROPERTIES 
        CXX_STANDART 20
        CXX_STANDARD_REQUIRED ON
)