add_executable(lirs lirs.cpp)

set_target_properties(lirs PROPERTIES 
    CXX_STANDART 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
