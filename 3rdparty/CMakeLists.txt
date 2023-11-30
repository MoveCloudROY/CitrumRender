cmake_minimum_required(VERSION 3.6.0)

project(third_party VERSION 0.1.0)

set(SPDLOG_LIB_NAME "spdlog")
add_library(${SPDLOG_LIB_NAME} INTERFACE)

target_include_directories(
    ${SPDLOG_LIB_NAME} 
        INTERFACE
    ${SPDLOG_LIB_NAME}/include
)

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

set(ASSIMP_LIB_NAME "assimp")
add_subdirectory(${ASSIMP_LIB_NAME})

set(IMGUI_LIB_NAME "imgui")
set(IMGUI_SRC_PATH "${IMGUI_LIB_NAME}")
set(IMGUI_INC_PATH "${IMGUI_LIB_NAME}")

set(IMGUI_BACKENDS_SRC_PATH "${IMGUI_LIB_NAME}/backends")
set(IMGUI_BACKENDS_INC_PATH "${IMGUI_LIB_NAME}/backends")

add_library(${IMGUI_LIB_NAME} STATIC 
            "${IMGUI_SRC_PATH}/imgui.cpp"
            "${IMGUI_SRC_PATH}/imgui_demo.cpp"
            "${IMGUI_SRC_PATH}/imgui_draw.cpp"
            "${IMGUI_SRC_PATH}/imgui_tables.cpp"
            "${IMGUI_SRC_PATH}/imgui_widgets.cpp"

            "${IMGUI_BACKENDS_SRC_PATH}/imgui_impl_glfw.cpp"
            "${IMGUI_BACKENDS_SRC_PATH}/imgui_impl_opengl3.cpp"
)

target_include_directories(${IMGUI_LIB_NAME} PUBLIC 
            ${IMGUI_INC_PATH}
            ${IMGUI_BACKENDS_INC_PATH}
)

set(IMTERM_LIB_NAME "imterm")
add_subdirectory(${IMTERM_LIB_NAME})