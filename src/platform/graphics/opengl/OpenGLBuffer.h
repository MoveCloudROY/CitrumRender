#pragma once

#include "renderer/Buffer.h"

namespace EG {

// Support STATIC draw or DYNAMIC draw with two Ctor
// Ref: https://www.reddit.com/r/opengl/comments/57i9cl/examples_of_when_to_use_gl_dynamic_draw/
// SetData(): set $data later when glBufferData()'s data item is NULL
//
class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetData(const void* data, uint32_t size) override;

    virtual const BufferLayout& GetLayout() const override { return layout_; }
    virtual void                SetLayout(const BufferLayout& layout) override { layout_ = layout; }

private:
    uint32_t     vboID_;
    BufferLayout layout_;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual uint32_t GetCount() const { return count_; }

private:
    uint32_t eboID_;
    uint32_t count_;
};

} // namespace EG