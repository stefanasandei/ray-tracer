cmake_minimum_required(VERSION 3.24)
project(external)

set(CMAKE_CXX_STANDARD 23)

add_library(external INTERFACE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" CACHE STRING "")

# glfw
add_subdirectory(glfw)
target_link_libraries(external INTERFACE glfw)
target_include_directories(external INTERFACE glfw/include)

# vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(external INTERFACE Vulkan::Vulkan)
target_include_directories(external INTERFACE ${Vulkan_INCLUDE_DIRS})

# imgui
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_vulkan.cpp)
target_link_libraries(external INTERFACE imgui)
target_include_directories(external INTERFACE imgui)
target_include_directories(imgui PUBLIC imgui glfw/include ${Vulkan_INCLUDE_DIRS})
target_include_directories(external INTERFACE imgui/backends)

# nri
add_subdirectory(nri)
target_link_libraries(external INTERFACE NRI)
target_include_directories(external INTERFACE nri/Include)

# metal util
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(QUARTZ_CORE QuartzCore REQUIRED)
    add_library(MetalUtility SHARED metal_util/MetalUtility.m)
    target_link_libraries(MetalUtility ${QUARTZ_CORE} ${COCOA_LIBRARY} glfw)
    target_link_libraries(external INTERFACE MetalUtility)
    target_include_directories(external INTERFACE metal_util)
endif ()
