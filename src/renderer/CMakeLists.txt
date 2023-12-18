cmake_minimum_required(VERSION 3.6.0)

set(RENDERER_INCLUDE_DIRS ..)
set(RENDERER_SOURCE_DIRS .)

file(GLOB_RECURSE RENDERER_SRC_FILES CONFIGURE_DEPENDS ${RENDERER_SOURCE_DIRS}/*.cpp)

add_library(renderer ${RENDERER_SRC_FILES})

target_link_libraries(renderer PUBLIC
    glfw
    core
    platform
)

target_include_directories(renderer PUBLIC ${RENDERER_INCLUDE_DIRS})
