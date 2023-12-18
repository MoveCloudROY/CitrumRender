#include "utils/texture.h"
#include <cstdint>
namespace EG::Utils {

TextureBuilder::TextureBuilder(const TextureType& type)
    : texType_(type) {
    // 生成纹理
    glGenTextures(1, &textureId_);
    glBindTexture(TextureTypeToGLType(type), textureId_);
}

TextureBuilder& TextureBuilder::Bind(uint32_t slot) {
    glBindTextureUnit(slot, textureId_);
    return *this;
}


TextureBuilder& TextureBuilder::SetProps() {
    // 为当前绑定的纹理对象设置环绕、过滤方式
    // GL_REPEAT            重复纹理图像
    // GL_MIRRORED_REPEAT   和GL_REPEAT一样，但每次重复图片是镜像放置的
    // GL_CLAMP_TO_EDGE     纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果
    // GL_CLAMP_TO_BORDER   超出的坐标为用户指定的边缘颜色。
    //
    // # WARNING #
    //    根据 https://stackoverflow.com/questions/26589683/access-violation-when-calling-gltextureparameteri-with-opengl-and-devil
    //    这里原先使用 glTextureParameteri(), 为 OpenGL 4.5 中的新入口点，
    //    这个想法是您可以在不绑定对象的情况下修改对象的状态。这可以通过减少 API 调用次数和驱动程序开销来提高效率。
    //    与相应的旧入口点相比，这些新入口点将对象名称作为第一个参数，而旧入口点将目标作为第一个参数。
    //    原先第一个参数为目标
    //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    新的则可以使用对象名称
    //       glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(TextureTypeToGLType(texType_), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(TextureTypeToGLType(texType_), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(TextureTypeToGLType(texType_), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(TextureTypeToGLType(texType_), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TextureTypeToGLType(texType_), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return *this;
}

TextureBuilder& TextureBuilder::Attach(const std::string& path, bool flip) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip);

    // stb 加载图片

    unsigned char* data =
        stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(TextureTypeToGLTarget(texType_) + textureIndex_, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (texType_ != TextureType::TEXTURE_CUBE_MAP)
            glGenerateMipmap(TextureTypeToGLTarget(texType_) + textureIndex_);
        ++textureIndex_;
    } else {
        spdlog::error("[Texture] Failed to load texture: {}", path);
    }
    stbi_image_free(data);

    return *this;
}

uint32_t TextureBuilder::Build() {
    return textureId_;
}

} // namespace EG::Utils
