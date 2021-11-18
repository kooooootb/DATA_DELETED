include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests test1.cpp)

target_link_libraries(runTests gtest gtest_main)
target_link_libraries(runTests lib)

include_directories(../..)