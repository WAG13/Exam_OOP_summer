cmake_minimum_required(VERSION 3.14)
project(Exam_OOP_summer)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build tests" ON)

add_executable(main main.cpp Data/DateTime.cpp)
target_include_directories(main PRIVATE ./)

if (BUILD_TESTS)
    add_executable(tests Test/Test.cpp Data/DateTime.cpp)
    target_include_directories(tests PRIVATE ./)
endif()
