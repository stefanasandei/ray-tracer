cmake_minimum_required(VERSION 3.24)
project(cli)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE CLI_HEADERS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CLI_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(cli ${CLI_HEADERS} ${CLI_SOURCES})

target_link_libraries(cli engine)

include_directories(.)
