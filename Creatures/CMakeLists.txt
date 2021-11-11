project(Creatures)

add_library(creatures STATIC
        Header_Creatures.h
        Creature.cpp
        Forager.cpp
        Operative.cpp
        Sentient.cpp
        Wild.cpp)