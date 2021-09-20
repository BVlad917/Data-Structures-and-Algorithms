cmake_minimum_required(VERSION 3.19)
project(Lab_2_Assignment_CLion)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab_2_Assignment_CLion
        ExtendedTest.cpp
        ExtendedTest.h ShortTest.cpp ShortTest.h ListIterator.cpp ListIterator.h SortedIteratedList.cpp SortedIteratedList.h App.cpp)
