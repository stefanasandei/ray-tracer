cmake_minimum_required(VERSION 3.24)
project(path_tracer)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_subdirectory(src/engine)
add_subdirectory(src/viewer)
