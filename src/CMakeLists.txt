# Create library from header file
add_library(read_from_csv INTERFACE)
target_include_directories(read_from_csv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(matplotplusplus)
include_directories(matplotplusplus/include)

# Create executables
add_executable(main main.cpp)

# Link libraries
target_link_libraries(main PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    matplot
)