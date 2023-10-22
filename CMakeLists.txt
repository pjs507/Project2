cmake_minimum_required(VERSION 2.8)
project(Project2 C)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_STANDARD 99)

add_executable(Project2 main.c lwp.c rr.c rr.h)
