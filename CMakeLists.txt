cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME 2LAB1)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task "task.cpp")