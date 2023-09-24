#ifndef EG_TEXTURE_H__
#define EG_TEXTURE_H__

#include <GL/glew.h>
#include <stb_image.h>
#include <spdlog/spdlog.h>

#include <stdint.h>

namespace Utils {

struct TextureOptions {
};


class TextureBuilder {
public:
    TextureBuilder();
    ~TextureBuilder();
    uint32_t build(const char* path);
    void     setOptions(const TextureOptions& options);

private:
    uint32_t textureId = -1;
};


} // namespace Utils

#endif /* EG_TEXTURE_H__ */
