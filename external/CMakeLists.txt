cmake_minimum_required(VERSION 3.24)
project(external)

set(CMAKE_CXX_STANDARD 23)

add_library(external INTERFACE)

# glfw
add_subdirectory(glfw)
target_link_libraries(external INTERFACE glfw)
include_directories(external INTERFACE glfw/include)
