#include "platform/graphics/opengl/OpenGLFramebuffer.h"

#include <core/Log.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace EG {

static const uint32_t s_MaxFramebufferSize = 8192;

namespace Utils {

static GLenum TextureTarget(bool multisampled) {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
    glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void BindTexture(bool multisampled, uint32_t id) {
    glBindTexture(TextureTarget(multisampled), id);
}

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // repeat for each edges of texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
    bool multisampled = samples > 1;
    if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    } else {
        // allocate immutable layout memory
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
}

static bool IsDepthFormat(FramebufferTextureFormat format) {
    switch (format) {
    case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
    default: return false;
    }

    return false;
}

static GLenum EG_FBTextureFormatToGL(FramebufferTextureFormat format) {
    switch (format) {
    case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
    case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
    default: EG_CORE_ASSERT(false, "[RHI] Unknow FrameBuffer texture format");
    }

    return 0;
}

} // namespace Utils

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : specification_(spec) {
    for (auto spec : specification_.Attachments) {
        if (!Utils::IsDepthFormat(spec.TextureFormat))
            colorAttachmentSpecifications_.emplace_back(spec);
        else
            depthAttachmentSpecification_ = spec;
    }

    FlushFrameBuffer();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &fboID_);
    glDeleteTextures(colorAttachments_.size(), colorAttachments_.data());
    glDeleteTextures(1, &depthAttachment_);
}

void OpenGLFramebuffer::FlushFrameBuffer() {
    if (fboID_) {
        glDeleteFramebuffers(1, &fboID_);
        glDeleteTextures(colorAttachments_.size(), colorAttachments_.data());
        glDeleteTextures(1, &depthAttachment_);

        colorAttachments_.clear();
        depthAttachment_ = 0;
    }
    // Create Framebuffer
    glCreateFramebuffers(1, &fboID_);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID_);

    bool multisample = specification_.Samples > 1;

    // Attachments
    if (colorAttachmentSpecifications_.size()) {
        colorAttachments_.resize(colorAttachmentSpecifications_.size());
        Utils::CreateTextures(multisample, colorAttachments_.data(), colorAttachments_.size());

        for (size_t i = 0; i < colorAttachments_.size(); i++) {
            Utils::BindTexture(multisample, colorAttachments_[i]);
            switch (colorAttachmentSpecifications_[i].TextureFormat) {
            // While GL_RGBA for internalFormat is still supported for backwards compatibility,
            // sized types are generally used for internalFormat in modern versions of OpenGL.
            // if you want to store the data as an 8-bit per component RGBA image, the value for internalFormat is GL_RGBA8
            case FramebufferTextureFormat::RGBA8:
                Utils::AttachColorTexture(colorAttachments_[i], specification_.Samples, GL_RGBA8, GL_RGBA, specification_.Width, specification_.Height, i);
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                Utils::AttachColorTexture(colorAttachments_[i], specification_.Samples, GL_R32I, GL_RED_INTEGER, specification_.Width, specification_.Height, i);
                break;
            default: break;
            }
        }
    }

    if (depthAttachmentSpecification_.TextureFormat != FramebufferTextureFormat::None) {
        Utils::CreateTextures(multisample, &depthAttachment_, 1);
        Utils::BindTexture(multisample, depthAttachment_);
        switch (depthAttachmentSpecification_.TextureFormat) {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            Utils::AttachDepthTexture(depthAttachment_, specification_.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, specification_.Width, specification_.Height);
            break;
        default: break;
        }
    }

    if (colorAttachments_.size() > 1) {
        // Specifies a list of color buffers to be drawn into
        EG_CORE_ASSERT(colorAttachments_.size() <= 4);
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(colorAttachments_.size(), buffers);
    } else if (colorAttachments_.empty()) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    EG_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID_);
    glViewport(0, 0, specification_.Width, specification_.Height);
}

void OpenGLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
        EG_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
        return;
    }
    specification_.Width  = width;
    specification_.Height = height;

    FlushFrameBuffer();
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
    EG_CORE_ASSERT(attachmentIndex < colorAttachments_.size());
    // Enable buffer for glReadPixels()
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
    EG_CORE_ASSERT(attachmentIndex < colorAttachments_.size());

    auto& spec = colorAttachmentSpecifications_[attachmentIndex];
    glClearTexImage(colorAttachments_[attachmentIndex], 0, Utils::EG_FBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}

} // namespace EG
