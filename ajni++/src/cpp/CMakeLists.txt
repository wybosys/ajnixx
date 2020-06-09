cmake_minimum_required(VERSION 3.1)
project(ajni++)

include_directories(..)

add_library(ajni++
ajni++.cpp
android.cpp
core.cpp
inspect.cpp
jre.cpp
variant.cpp
)

set_target_properties(ajni++
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
