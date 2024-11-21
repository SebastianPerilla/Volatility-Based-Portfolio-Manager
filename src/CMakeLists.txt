# Create library from header file
add_library(read_from_csv INTERFACE)
target_include_directories(read_from_csv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Create executable
add_executable(main main.cpp)
add_executable(volatility volatility.cpp)
add_executable(volatility volatility.cpp)


