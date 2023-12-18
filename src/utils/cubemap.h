#ifndef EG_CUBEMAP_H__
#define EG_CUBEMAP_H__

#include "utils/shader.h"

#include <initializer_list>
#include <string>
#include <cstdint>

namespace EG::Utils {


class CubeMap {
public:
    CubeMap(std::initializer_list<std::string>);
    ~CubeMap() = default;

    void    SetFaces(std::initializer_list<std::string>);
    Shader& GetShader() { return shader_; }
    void    Draw(const glm::mat4& v, const glm::mat4& p);

private:
    uint32_t textureId_;
    uint32_t cubeVaoId_;
    Shader   shader_;
};

} // namespace EG::Utils

#endif