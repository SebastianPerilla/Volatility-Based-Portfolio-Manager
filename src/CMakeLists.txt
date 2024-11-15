add_library(monte_carlo SHARED monte_carlo.h)
set_target_properties(monte_carlo PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(monte_carlo)

