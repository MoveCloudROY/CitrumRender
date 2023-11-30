#pragma once

#include <memory>
#include <vector>

namespace EG {

enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::None: return 0;
    case ShaderDataType::Float: return 4;
    case ShaderDataType::Float2: return 4 * 2;
    case ShaderDataType::Float3: return 4 * 3;
    case ShaderDataType::Float4: return 4 * 4;
    case ShaderDataType::Mat3: return 4 * 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4 * 4;
    case ShaderDataType::Int: return 4;
    case ShaderDataType::Int2: return 4 * 2;
    case ShaderDataType::Int3: return 4 * 3;
    case ShaderDataType::Int4: return 4 * 4;
    case ShaderDataType::Bool: return 1;
    }
    return 0;
}

// Describe each elements in Layout
struct BufferElement {
    std::string    name;
    ShaderDataType type;
    uint32_t       size;
    size_t         offset;
    bool           normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {
    }

    uint32_t GetComponentCount() const {
        switch (type) {
        case ShaderDataType::None: return 0;
        case ShaderDataType::Float: return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Mat3: return 3; // 3* float3
        case ShaderDataType::Mat4: return 4; // 4* float4
        case ShaderDataType::Int: return 1;
        case ShaderDataType::Int2: return 2;
        case ShaderDataType::Int3: return 3;
        case ShaderDataType::Int4: return 4;
        case ShaderDataType::Bool: return 1;
        }

        return 0;
    }
};

// Combine elements into a array
// Just describe the struct of buffer
class BufferLayout {
public:
    BufferLayout() {}

    BufferLayout(std::initializer_list<BufferElement> elements)
        : elements_(elements) {
        CalculateOffsetsAndStride();
    }

    uint32_t                          GetStride() const { return stride_; }
    const std::vector<BufferElement>& GetElements() const { return elements_; }

    std::vector<BufferElement>::iterator       begin() { return elements_.begin(); }
    std::vector<BufferElement>::iterator       end() { return elements_.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }

private:
    void CalculateOffsetsAndStride() {
        size_t offset = 0;
        stride_       = 0;
        for (auto& element : elements_) {
            element.offset = offset;
            offset += element.size;
            stride_ += element.size;
        }
    }

private:
    std::vector<BufferElement> elements_;
    uint32_t                   stride_ = 0;
};


// Interface:
// describe layout in BufferLayout
// SetData() api for set actually data to layout
// Bind() api for Bind VBO State
class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& GetLayout() const                     = 0;
    virtual void                SetLayout(const BufferLayout& layout) = 0;

    static std::shared_ptr<VertexBuffer> Create(uint32_t size);
    static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
};


// Interface:
// describe layout in BufferLayout
// SetData() api for set actually data to layout
// Bind() api for Bind EBO State
class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
};

} // namespace EG
