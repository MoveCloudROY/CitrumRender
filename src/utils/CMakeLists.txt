cmake_minimum_required(VERSION 3.6.0)

project(utils VERSION 0.1.0)

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} ${FILES})

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} stb)
target_link_libraries(${PROJECT_NAME} spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC include)

