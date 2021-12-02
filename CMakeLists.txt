cmake_minimum_required(VERSION 3.18)
project(lab4)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../files)

add_executable(lab4 main.cpp Game.cpp Game.h dialog.cpp dialog.h EditTool.cpp EditTool.h Map/Map.h)

include_directories(Creatures)
include_directories(Items)
include_directories(Level)
include_directories(Map)

add_subdirectory(Creatures)
add_subdirectory(Items)
add_subdirectory(Level)
#add_subdirectory(Tests)

add_library(lib INTERFACE)
target_link_libraries(lib INTERFACE level creatures items)

target_link_libraries(lab4 lib)





#sfml libs
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory(sfml)
include_directories(sfml/include)

target_link_libraries(lab4 sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
