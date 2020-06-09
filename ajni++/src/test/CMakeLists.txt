cmake_minimum_required(VERSION 3.1)

include_directories(../cpp)

add_library(tester SHARED
test.cpp
)

target_link_libraries(tester ajni++)

set_target_properties(tester
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
