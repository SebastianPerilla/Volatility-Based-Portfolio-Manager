cmake_minimum_required(VERSION 3.20)
project(ExpressionProblems)

set(CMAKE_CXX_STANDARD 17) # This sets the C++ standard to C++17
set(CMAKE_CXX_STANDARD_REQUIRED True) # This tells CMake that the stdandard must be available

# We place the executables in the bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# We place the libraries in the lib directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# We normally have a CMakeLists.txt file in each directory that has something that needs to be built
add_subdirectory(include)
add_subdirectory(src) # This line tells CMake to go to the src directory and run the CMakeLists.txt file there
enable_testing() # This line allows to call ctest after compilation
add_subdirectory(tests)


