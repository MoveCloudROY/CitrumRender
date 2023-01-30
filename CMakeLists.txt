cmake_minimum_required(VERSION 3.0.0)
project(OpenGL VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (POLICY CMP0072)
    set(OpenGL_GL_PREFERENCE GLVND)
endif(POLICY CMP0072)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} main.cpp data.cpp shader.cpp)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)

# target_compile_options(${PROJECT_NAME}  PRIVATE -fno-omit-frame-pointer)
# target_compile_options(${PROJECT_NAME}  PRIVATE -fsanitize=address)
# target_link_options(${PROJECT_NAME}  PRIVATE -fsanitize=address)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC=)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
