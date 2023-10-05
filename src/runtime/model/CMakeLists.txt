set(MODULE_NAME model)

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS src/*.cpp)

add_library(${MODULE_NAME} ${FILES})

target_link_libraries(${MODULE_NAME} 
        OpenGL::GL
        stb
        assimp
        glm
        spdlog
        utils
)

target_include_directories(${MODULE_NAME} PUBLIC include)
