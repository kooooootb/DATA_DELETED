project(Items)

add_library(items STATIC
        Header_Items.h
        AmmoContainer.cpp
        Gun.cpp
        HealthKit.cpp
        Item.cpp
        Table.cpp)

include_directories(sfml/include)

target_link_libraries(items sfml-graphics sfml-window sfml-system)