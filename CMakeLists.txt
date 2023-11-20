cmake_minimum_required(VERSION 3.26)
project(COMP4300)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE EXAMPLE_FILES examples/*cpp)
file(GLOB_RECURSE ENGINE_FILES engine/*.cpp)

add_executable(COMP4300 main.cpp ${ENGINE_FILES} ${EXAMPLE_FILES}
        examples/mario/MarioGame.cpp
        examples/mario/MarioGame.h)

find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
target_link_libraries(COMP4300 sfml-window sfml-graphics)