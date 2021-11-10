cmake_minimum_required(VERSION 3.18)
project(lab4)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab4 main.cpp)

include_directories(Creatures)
include_directories(Items)
include_directories(Level)

add_subdirectory(Creatures)
add_subdirectory(Items)
add_subdirectory(Level)

target_link_libraries(lab4 creatures)
target_link_libraries(lab4 items)
target_link_libraries(lab4 level)