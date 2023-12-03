#pragma once

#include "utils/buffer.h"

namespace EG::Utils {

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    VertexArray& Bind();
    void         Unbind() const;

    VertexArray& SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    VertexArray& SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

    const std::shared_ptr<VertexBuffer>& GetVertexBuffers() const {
        return vertexBuffer_;
    }
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const {
        return indexBuffer_;
    }

private:
    uint32_t                      rendererID_;
    uint32_t                      vertexBufferIndex_ = 0;
    std::shared_ptr<VertexBuffer> vertexBuffer_;
    std::shared_ptr<IndexBuffer>  indexBuffer_;
};

} // namespace EG::Utils
