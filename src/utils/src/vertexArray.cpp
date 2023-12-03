#include "utils/vertexArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

namespace EG::Utils {

static GLenum ShaderDataTypeToBaseType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float: return GL_FLOAT;
    case ShaderDataType::Vecf2: return GL_FLOAT;
    case ShaderDataType::Vecf3: return GL_FLOAT;
    case ShaderDataType::Vecf4: return GL_FLOAT;
    case ShaderDataType::Mat3: return GL_FLOAT;
    case ShaderDataType::Mat4: return GL_FLOAT;
    case ShaderDataType::Int: return GL_INT;
    case ShaderDataType::Veci2: return GL_INT;
    case ShaderDataType::Veci3: return GL_INT;
    case ShaderDataType::Veci4: return GL_INT;
    case ShaderDataType::Bool: return GL_BOOL;
    default: break;
    }
    spdlog::error("Unknown ShaderDataType!");
    return 0;
}

VertexArray::VertexArray() {

    glCreateVertexArrays(1, &rendererID_);
}

VertexArray::~VertexArray() {

    glDeleteVertexArrays(1, &rendererID_);
}

VertexArray& VertexArray::Bind() {

    glBindVertexArray(rendererID_);
    return *this;
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

VertexArray& VertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {

    if (!vertexBuffer->GetLayout().GetElements().size())
        spdlog::error("Vertex Buffer has no layout!");

    glBindVertexArray(rendererID_);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        switch (element.type) {
        case ShaderDataType::Float:
        case ShaderDataType::Vecf2:
        case ShaderDataType::Vecf3:
        case ShaderDataType::Vecf4: {
            glEnableVertexAttribArray(vertexBufferIndex_);
            glVertexAttribPointer(vertexBufferIndex_, element.GetComponentCount(), ShaderDataTypeToBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
            spdlog::info("VA:  Index: {}, size: {}, stride: {}, offset : {} ", vertexBufferIndex_, element.GetComponentCount(), layout.GetStride(), element.offset);
            vertexBufferIndex_++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Veci2:
        case ShaderDataType::Veci3:
        case ShaderDataType::Veci4:
        case ShaderDataType::Bool: {
            glEnableVertexAttribArray(vertexBufferIndex_);
            glVertexAttribIPointer(vertexBufferIndex_, element.GetComponentCount(), ShaderDataTypeToBaseType(element.type), layout.GetStride(), (const void*)element.offset);
            vertexBufferIndex_++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
            uint8_t count = element.GetComponentCount();
            for (uint8_t i = 0; i < count; i++) {
                glEnableVertexAttribArray(vertexBufferIndex_);
                glVertexAttribPointer(vertexBufferIndex_, count, ShaderDataTypeToBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(element.offset + sizeof(float) * count * i));
                glVertexAttribDivisor(vertexBufferIndex_, 1);
                vertexBufferIndex_++;
            }
            break;
        }
        default:
            spdlog::error("Unknown ShaderDataType!");
        }
    }

    vertexBuffer_ = vertexBuffer;
    return *this;
}

VertexArray& VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {

    glBindVertexArray(rendererID_);
    indexBuffer->Bind();

    indexBuffer_ = indexBuffer;
    return *this;
}

} // namespace EG::Utils
