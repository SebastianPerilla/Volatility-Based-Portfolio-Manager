add_library(transform SHARED transform.h)
set_target_properties(transform PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(transform)

