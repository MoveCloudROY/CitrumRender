cmake_minimum_required(VERSION 3.0.0)

project(utils VERSION 0.1.0)
add_library(${PROJECT_NAME} data.cpp shader.cpp)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
