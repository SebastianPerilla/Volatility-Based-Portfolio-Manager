# Create library from header file
add_library(read_from_csv INTERFACE)
target_include_directories(read_from_csv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Create executable
add_executable(montecarlo montecarlo.cpp)

# Link libraries
target_link_libraries(montecarlo PUBLIC read_from_csv Eigen3::Eigen)
