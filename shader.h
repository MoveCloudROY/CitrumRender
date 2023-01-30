#pragma once 

#include <GL/glew.h>

#include <iostream>


namespace Utils {
    unsigned int createShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource);
}