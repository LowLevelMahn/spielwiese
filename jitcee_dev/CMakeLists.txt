cmake_minimum_required(VERSION 3.20.0)
project(llvm-orc-jit)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILTER "includes")
set(SOURCE_FILTER "sources")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP4 /external:W0 /external:anglebrackets")
 
include(macros.cmake)

add_subdirectory (jitcee)
add_subdirectory (jitcee_lib)
add_subdirectory (simple_test)
