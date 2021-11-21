project(Level)

add_library(level STATIC
        Level.cpp
        Level.h
        Cell.cpp
        CellIt.cpp
        CellIt.h)

include_directories(sfml/include)

target_link_libraries(level sfml-graphics sfml-window sfml-system)