#ifndef EG_TEXTURE_H__
#define EG_TEXTURE_H__

#include <GL/glew.h>
#include <stb/stb_image.h>
#include <spdlog/spdlog.h>

#include <stdint.h>
#include <string>

namespace Utils {

struct TextureOptions {
};


class TextureBuilder {
public:
    TextureBuilder();
    ~TextureBuilder();
    uint32_t build(const std::string& path, bool flip = true);
    void     setOptions(const TextureOptions& options);

private:
    uint32_t textureId = -1;
};


} // namespace Utils

#endif /* EG_TEXTURE_H__ */
