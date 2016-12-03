cmake_minimum_required(VERSION 3.6)
project(memory_file_simulation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES memory_file_test.c mfile.c mfile.h)
add_executable(memory_file_simulation ${SOURCE_FILES})