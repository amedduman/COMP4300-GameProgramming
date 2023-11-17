cmake_minimum_required(VERSION 3.26)
project(course)

set(CMAKE_CXX_STANDARD 23)

add_executable(course main.cpp)

find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
target_link_libraries(course sfml-window sfml-graphics)
