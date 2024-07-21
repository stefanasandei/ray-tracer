cmake_minimum_required(VERSION 3.24)
project(external)

set(CMAKE_CXX_STANDARD 23)

add_library(glad STATIC ./src/glad.cpp)

target_include_directories(glad PUBLIC include)

