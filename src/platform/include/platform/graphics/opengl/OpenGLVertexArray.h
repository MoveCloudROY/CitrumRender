#pragma once

#include "renderer/VertexArray.h"

namespace EG {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
        return vertexBuffers_;
    }
    virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {
        return indexBuffer_;
    }

private:
    uint32_t                                   rendererID_;
    uint32_t                                   vertexBufferIndex_ = 0;
    std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers_;
    std::shared_ptr<IndexBuffer>               indexBuffer_;
};

} // namespace EG
