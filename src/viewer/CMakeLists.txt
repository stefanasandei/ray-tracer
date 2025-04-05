cmake_minimum_required(VERSION 3.24)
project(viewer)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE VIEWER_HEADERS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE VIEWER_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(viewer ${VIEWER_HEADERS} ${VIEWER_SOURCES}
        core/panel.cpp)

target_link_libraries(viewer engine)

include_directories(.)
