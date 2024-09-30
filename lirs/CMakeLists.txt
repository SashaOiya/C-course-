cmake_minimum_required(VERSION 3.11)

project("lirs_cache")

set(SOURCES
        lirs.cpp
)
set (HEADERS
        lirs.hpp
)

add_executable(lirs ${SOURCES} ${HEADERS})

option(MyOption "MyOption" OFF)

set_target_properties(
        lirs PROPERTIES 
        CXX_STANDART 20
        CXX_STANDARD_REQUIRED ON
)