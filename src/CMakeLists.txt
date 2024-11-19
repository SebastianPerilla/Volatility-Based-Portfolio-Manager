# Create library from header file
add_library(read_from_csv INTERFACE)
target_include_directories(read_from_csv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Create executables
add_executable(montecarlo montecarlo.cpp)
add_executable(extractor extractor.cpp)

# Link libraries
target_link_libraries(montecarlo PUBLIC read_from_csv Eigen3::Eigen)
target_link_libraries(extractor PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)
