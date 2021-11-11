cmake_minimum_required(VERSION 3.18)
project(lab4)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../files)

add_executable(lab4 main.cpp Game.cpp Game.h)

include_directories(Creatures)
include_directories(Items)
include_directories(Level)
include_directories(Map)

add_subdirectory(Creatures)
add_subdirectory(Items)
add_subdirectory(Level)
add_subdirectory(Map)

target_link_libraries(lab4 creatures)
target_link_libraries(lab4 items)
target_link_libraries(lab4 level)
target_link_libraries(lab4 map)

#set(BUILD_SHARED_LIBS FALSE)
#
#add_subdirectory(sfml)
#include_directories(sfml/include)
#
#target_link_libraries(lab4 sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
