cmake_minimum_required(VERSION 3.19)
project(Lab1_Assignment_CLion)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab1_Assignment_CLion
        ExtendedTest.cpp
        ExtendedTest.h ShortTest.cpp ShortTest.h SortedSet.cpp SortedSet.h SortedSetIterator.cpp SortedSetIterator.h App.cpp)
