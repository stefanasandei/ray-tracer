cmake_minimum_required(VERSION 3.24)
project(engine)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE ENGINE_HEADERS ${PROJECT_SOURCE_DIR}/../../include/*.hpp)
file(GLOB_RECURSE ENGINE_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(engine ${ENGINE_SOURCES} ${ENGINE_HEADERS}
        ../cli/main.cpp)

target_link_libraries(engine external)
