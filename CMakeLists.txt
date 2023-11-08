cmake_minimum_required(VERSION 3.26)
project(COMP4300_GameProgramming)

set(CMAKE_CXX_STANDARD 20)

add_executable(COMP4300_GameProgramming main.cpp)

find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
target_link_libraries(COMP4300_GameProgramming sfml-window sfml-graphics)