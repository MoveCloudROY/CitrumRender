cmake_minimum_required(VERSION 3.0.0)

set(SPDLOG_LIB_NAME "spdlog")
add_subdirectory(${SPDLOG_LIB_NAME})

set(DOCTEST_LIB_NAME "doctest")
add_subdirectory(${DOCTEST_LIB_NAME})

set(GLFW_LIB_NAME "glfw")
add_subdirectory(${GLFW_LIB_NAME})

set(GLM_LIB_NAME "glm")
add_subdirectory(${GLM_LIB_NAME})

set(STB_LIB_NAME "stb")
set(STB_SRC_PATH "${STB_LIB_NAME}")
set(STB_INC_PATH "${STB_LIB_NAME}/include")

add_library(${STB_LIB_NAME} STATIC ${STB_SRC_PATH}/stb_image.cpp)
target_include_directories(${STB_LIB_NAME} PUBLIC ${STB_INC_PATH})
