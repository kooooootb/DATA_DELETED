project(Creatures)

add_library(creatures STATIC
        Creature.cpp
        Forager.cpp
        Operative.cpp
        Sentient.cpp
        Wild.cpp)

include_directories(sfml/include)

target_link_libraries(creatures sfml-graphics sfml-window sfml-system)