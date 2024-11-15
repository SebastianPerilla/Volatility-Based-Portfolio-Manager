include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(test_split test_split.cpp)
add_executable(test_mytype test_mytype.cpp)
add_executable(test_sum test_sum.cpp)
add_executable(test_pair test_pair.cpp)
add_executable(test_transform test_transform.cpp)
add_executable(test_raw_pointer test_raw_pointer.cpp)

target_include_directories(test_split PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_mytype PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_sum PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_pair PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_transform PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_raw_pointer PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link each executable to the necessary libraries
target_link_libraries(test_split PRIVATE GTest::gtest_main)
target_link_libraries(test_mytype PRIVATE GTest::gtest_main)
target_link_libraries(test_sum PRIVATE GTest::gtest_main)
target_link_libraries(test_pair PRIVATE GTest::gtest_main)
target_link_libraries(test_transform PRIVATE GTest::gtest_main)
target_link_libraries(test_raw_pointer PRIVATE GTest::gtest_main)

gtest_discover_tests(test_split)
gtest_discover_tests(test_mytype)
gtest_discover_tests(test_sum)
gtest_discover_tests(test_pair)
gtest_discover_tests(test_transform)
gtest_discover_tests(test_raw_pointer)