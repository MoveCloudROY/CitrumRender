cmake_minimum_required(VERSION 3.6.0)

set(PLATFORM_INCLUDE_DIRS ..)
set(PLATFORM_SOURCE_DIRS .)

file(GLOB_RECURSE PLATFORM_SRC_FILES CONFIGURE_DEPENDS ${PLATFORM_SOURCE_DIRS}/*.cpp)

add_library(platform ${PLATFORM_SRC_FILES})

target_link_libraries(platform PUBLIC
    renderer
    glfw
    core
)

target_include_directories(platform PUBLIC ${PLATFORM_INCLUDE_DIRS})
