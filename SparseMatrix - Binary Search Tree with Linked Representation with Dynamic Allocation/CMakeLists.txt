cmake_minimum_required(VERSION 3.19)
project(Lab5_CLion)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab5_CLion
        ExtendedTest.cpp
        ExtendedTest.h ShortTest.cpp ShortTest.h Matrix.cpp Matrix.h MatrixIterator.cpp MatrixIterator.h App.cpp)
