#pragma once

#include "renderer/Framebuffer.h"
#include <core/Log.h>

namespace EG {

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    void FlushFrameBuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual int  ReadPixel(uint32_t attachmentIndex, int x, int y) override;

    virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
        EG_CORE_ASSERT(index < colorAttachments_.size());
        return colorAttachments_[index];
    }

    virtual const FramebufferSpecification& GetSpecification() const override { return specification_; }

private:
    uint32_t                 fboID_ = 0;
    FramebufferSpecification specification_;

    std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications_;
    FramebufferTextureSpecification              depthAttachmentSpecification_ = FramebufferTextureFormat::None;

    std::vector<uint32_t> colorAttachments_;
    uint32_t              depthAttachment_ = 0;
};

} // namespace EG
