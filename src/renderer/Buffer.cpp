#include "renderer/Buffer.h"

#include "renderer/RHI.h"

#include <platform/graphics/opengl/OpenGLBuffer.h>

#include <core/Log.h>

namespace EG {

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (RHI::GetAPI()) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
    }

    return nullptr;
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (RHI::GetAPI()) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (RHI::GetAPI()) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
    }

    return nullptr;
}

} // namespace EG