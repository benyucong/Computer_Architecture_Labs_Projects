cmake_minimum_required(VERSION 3.22)
project(exercise1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(exercise1
        ex1.c
        ex1.h
        test_ex1.c)
