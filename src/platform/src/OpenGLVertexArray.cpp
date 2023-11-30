#include "platform/graphics/opengl/OpenGLVertexArray.h"

#include <core/Log.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace EG {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float: return GL_FLOAT;
    case ShaderDataType::Float2: return GL_FLOAT;
    case ShaderDataType::Float3: return GL_FLOAT;
    case ShaderDataType::Float4: return GL_FLOAT;
    case ShaderDataType::Mat3: return GL_FLOAT;
    case ShaderDataType::Mat4: return GL_FLOAT;
    case ShaderDataType::Int: return GL_INT;
    case ShaderDataType::Int2: return GL_INT;
    case ShaderDataType::Int3: return GL_INT;
    case ShaderDataType::Int4: return GL_INT;
    case ShaderDataType::Bool: return GL_BOOL;
    default: EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }

    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {

    glCreateVertexArrays(1, &rendererID_);
}

OpenGLVertexArray::~OpenGLVertexArray() {

    glDeleteVertexArrays(1, &rendererID_);
}

void OpenGLVertexArray::Bind() const {

    glBindVertexArray(rendererID_);
}

void OpenGLVertexArray::Unbind() const {

    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {

    EG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(rendererID_);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        switch (element.type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4: {
            glEnableVertexAttribArray(vertexBufferIndex_);
            glVertexAttribPointer(vertexBufferIndex_, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
            vertexBufferIndex_++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool: {
            glEnableVertexAttribArray(vertexBufferIndex_);
            glVertexAttribIPointer(vertexBufferIndex_, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type), layout.GetStride(), (const void*)element.offset);
            vertexBufferIndex_++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
            uint8_t count = element.GetComponentCount();
            for (uint8_t i = 0; i < count; i++) {
                glEnableVertexAttribArray(vertexBufferIndex_);
                glVertexAttribPointer(vertexBufferIndex_, count, ShaderDataTypeToOpenGLBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(element.offset + sizeof(float) * count * i));
                glVertexAttribDivisor(vertexBufferIndex_, 1);
                vertexBufferIndex_++;
            }
            break;
        }
        default:
            EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
        }
    }

    vertexBuffers_.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {

    glBindVertexArray(rendererID_);
    indexBuffer->Bind();

    indexBuffer_ = indexBuffer;
}

} // namespace EG
