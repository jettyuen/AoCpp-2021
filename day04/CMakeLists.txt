cmake_minimum_required(VERSION 3.21)
project(AoC_2021)

set(CMAKE_CXX_STANDARD 14)

include_directories(day04)

add_executable(AoC_2021
        board.cpp
        board.hpp
        game.cpp
        game.hpp
        main.cpp
        puzzle_input.txt)
