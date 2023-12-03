#ifndef EG_TEXTURE_H__
#define EG_TEXTURE_H__

#include <GL/glew.h>
#include <stb/stb_image.h>
#include <spdlog/spdlog.h>

#include <stdint.h>
#include <string>
#include <memory>

namespace EG::Utils {

enum class TextureType {
    TEXTURE_2D,
    TEXTURE_CUBE_MAP,
};

inline GLenum TextureTypeToGLType(const TextureType& type) {

    switch (type) {
    case TextureType::TEXTURE_2D: return GL_TEXTURE_2D;
    case TextureType::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
    }
}

inline GLenum TextureTypeToGLTarget(const TextureType& type) {

    switch (type) {
    case TextureType::TEXTURE_2D: return GL_TEXTURE_2D;
    case TextureType::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }
}


class TextureBuilder {
public:
    TextureBuilder(const TextureType& type);
    ~TextureBuilder() = default;
    TextureBuilder& Bind(uint32_t slot = 0);
    TextureBuilder& SetProps();
    TextureBuilder& Attach(const std::string& path, bool flip = true);
    TextureBuilder& Attach(const std::vector<std::string>& path, bool flip = true);

    uint32_t Build();

private:
    uint32_t    textureId_;
    uint32_t    textureIndex_ = 0;
    TextureType texType_      = TextureType::TEXTURE_2D;
};


} // namespace EG::Utils

#endif /* EG_TEXTURE_H__ */
