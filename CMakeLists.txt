cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Werror")

project(tape_sorting VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(src/interface)
add_subdirectory(src/tape)

add_library(${PROJECT_NAME}_objs 
    $<TARGET_OBJECTS:interface> 
    $<TARGET_OBJECTS:tape> 
    )

add_executable(${PROJECT_NAME} src/main.cpp)