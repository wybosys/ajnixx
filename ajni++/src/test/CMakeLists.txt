cmake_minimum_required(VERSION 3.1)

include_directories(../cpp)

add_library(test_ajni++ SHARED
test.cpp
)

target_link_libraries(test_ajni++ ajni++)

set_target_properties(test_ajni++
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
