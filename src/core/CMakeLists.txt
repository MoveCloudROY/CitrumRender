cmake_minimum_required(VERSION 3.6.0)

set(CORE_INCLUDE_DIRS include)
set(CORE_SOURCE_DIRS src)

file(GLOB_RECURSE CORE_SRC_FILES CONFIGURE_DEPENDS ${CORE_SOURCE_DIRS}/*.cpp)

add_library(core ${CORE_SRC_FILES})

target_link_libraries(core PUBLIC
    spdlog
    imgui
    ImTerm
    platform
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "[Core] Enable Debug")
    target_compile_definitions(core PUBLIC -DEG_DEBUG)
endif()

target_include_directories(core PUBLIC ${CORE_INCLUDE_DIRS})
