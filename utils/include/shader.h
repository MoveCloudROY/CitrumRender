#ifndef MY_SHADER_H__
#define MY_SHADER_H__

#include <GL/glew.h>
#include <iostream>

namespace Utils {
    unsigned int createShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource);
}

#endif /* MY_SHADER_H__ */