cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(utils)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)

target_link_libraries(${PROJECT_NAME} utils)

# target_compile_options(${PROJECT_NAME}  PRIVATE -fno-omit-frame-pointer)
# target_compile_options(${PROJECT_NAME}  PRIVATE -fsanitize=address)
# target_link_options(${PROJECT_NAME}  PRIVATE -fsanitize=address)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
