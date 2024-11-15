add_library(split SHARED split.h)
set_target_properties(split PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(split)


add_library(is_raw_pointer SHARED is_raw_pointer.h)
set_target_properties(is_raw_pointer PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(is_raw_pointer)

add_library(mytype SHARED mytype.h)
set_target_properties(mytype PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(mytype)

add_library(pair SHARED pair.h)
set_target_properties(pair PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(pair)

add_library(sum SHARED sum.h)
set_target_properties(sum PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(sum)

add_library(transform SHARED transform.h)
set_target_properties(transform PROPERTIES LINKER_LANGUAGE CXX) # need to specify C++ as a linker language
target_link_libraries(transform)

