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

#add_executable(test_monte_carlo test_monte_carlo.cpp)
#add_executable(test_read_from_csv test_read_from_csv.cpp)

#target_include_directories(test_monte_carlo PRIVATE ${CMAKE_SOURCE_DIR}/src)
#target_include_directories(test_read_from_csv PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link each executable to the necessary libraries
#target_link_libraries(test_monte_carlo PRIVATE GTest::gtest_main)
#target_link_libraries(test_read_from_csv PRIVATE GTest::gtest_main)


#gtest_discover_tests(test_monte_carlo)
#gtest_discover_tests(test_read_from_csv)
