cmake_minimum_required(VERSION 3.0.0)

add_library(editor src/Layer.cpp)

target_link_libraries(editor PUBLIC
    glfw
    spdlog
    imgui
)

target_include_directories(editor PUBLIC include)


