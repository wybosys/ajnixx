cmake_minimum_required(VERSION 3.4.1)

add_library(ajni
        SHARED
        ajni.cpp
        android.cpp
        test.cpp)

find_library(log-lib
        log)

target_link_libraries(ajni
        ${log-lib})