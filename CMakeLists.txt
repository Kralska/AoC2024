cmake_minimum_required(VERSION 3.30.0)
project(advent_of_code_2024 VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED 20)

include_directories(include/)

add_library(array2d STATIC array2d.cpp)
add_library(vector STATIC vector.cpp)
add_library(shortest_path STATIC shortest_path.cpp)


add_executable(day01 day01.cpp)
add_executable(day02 day02.cpp)
add_executable(day03 day03.cpp)
add_executable(day04 day04.cpp)
add_executable(day05 day05.cpp)

add_executable(day11 day11.cpp)
add_executable(day12 day12.cpp)
target_link_libraries(day12 array2d vector)
add_executable(day13 day13.cpp)
add_executable(day14 day14.cpp)
target_link_libraries(day14 array2d vector)
add_executable(day15 day15.cpp)
target_link_libraries(day15 array2d)
add_executable(day16 day16.cpp)
target_link_libraries(day16 array2d)
add_executable(day17 day17.cpp)
add_executable(day18 day18.cpp)
target_link_libraries(day18 PRIVATE shortest_path array2d)