add_executable(ideal ideal.cpp)

set_target_properties(ideal PROPERTIES 
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
