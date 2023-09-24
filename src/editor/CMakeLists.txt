cmake_minimum_required(VERSION 3.6.0)

set(EDITOR_INCLUDE_DIRS include)
set(EDITOR_SOURCE_DIRS src)

file(GLOB_RECURSE EDITOR_SRC_FILES CONFIGURE_DEPENDS ${EDITOR_SOURCE_DIRS}/*.cpp)

add_library(editor ${EDITOR_SRC_FILES})

target_link_libraries(editor PUBLIC
    glfw
    spdlog
    imgui
)

target_include_directories(editor PUBLIC ${EDITOR_INCLUDE_DIRS})
