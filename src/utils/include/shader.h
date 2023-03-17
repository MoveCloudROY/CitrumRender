#ifndef EG_SHADER_H__
#define EG_SHADER_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace Utils {

class Shader {
public:
    // 构造器读取并构建着色器
    Shader(const char * vertexPath, const char * fragmentPath);
    // 使能程序
    void use();
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void set3Float(const std::string &name, float a, float b, float c) const;
    void setMatrix4f(const std::string &name, const glm::mat4& mat) const;
    
public:
    // 程序ID
    unsigned int ID;
};


}

#endif /* EG_SHADER_H__ */