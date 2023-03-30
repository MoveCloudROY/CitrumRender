cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(utils)
add_subdirectory(editor)

add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            OpenGL::GL
            glfw
            GLEW::GLEW
            utils
            stb
            spdlog
            imgui
            editor
)

# target_compile_options(${PROJECT_NAME}  PRIVATE -fno-omit-frame-pointer)
# target_compile_options(${PROJECT_NAME}  PRIVATE -fsanitize=address)
# target_link_options(${PROJECT_NAME}  PRIVATE -fsanitize=address)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
