cmake_minimum_required(VERSION 3.1)

add_library(cross STATIC
cross.cpp
str.cpp
sys.cpp
timer.cpp
datetime.cpp
)

set_target_properties(cross
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
