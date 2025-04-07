cmake_minimum_required(VERSION 3.24)
project(external)

set(CMAKE_CXX_STANDARD 23)

add_library(external INTERFACE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" CACHE STRING "")

# glfw
add_subdirectory(glfw)
target_link_libraries(external INTERFACE glfw)
target_include_directories(external INTERFACE glfw/include)

# glad
add_subdirectory(glad)
target_link_libraries(external INTERFACE glad)
target_include_directories(external INTERFACE glad/include)

# imgui
add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_link_libraries(external INTERFACE imgui)
target_include_directories(external INTERFACE imgui)
target_include_directories(imgui PUBLIC imgui glfw/include glad)
target_include_directories(external INTERFACE imgui/backends)

# stb
target_include_directories(external INTERFACE stb)

# glm
add_subdirectory(glm)
target_link_libraries(external INTERFACE glm)
target_include_directories(external INTERFACE glm)

# ImGuiFileDialog
add_subdirectory(imguifiledialog)
target_link_libraries(ImGuiFileDialog PUBLIC imgui)
target_include_directories(ImGuiFileDialog PUBLIC imgui)
target_link_libraries(external INTERFACE ImGuiFileDialog)
target_include_directories(external INTERFACE imguifiledialog)
