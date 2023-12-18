#pragma once

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <spdlog/spdlog.h> // TODO: TEMP

namespace EG::Utils {

enum class ShaderDataType {
    None = 0,
    Float,
    Vecf2,
    Vecf3,
    Vecf4,
    Mat3,
    Mat4,
    Int,
    Veci2,
    Veci3,
    Veci4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::None: return 0;
    case ShaderDataType::Float: return 4;
    case ShaderDataType::Vecf2: return 4 * 2;
    case ShaderDataType::Vecf3: return 4 * 3;
    case ShaderDataType::Vecf4: return 4 * 4;
    case ShaderDataType::Mat3: return 4 * 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4 * 4;
    case ShaderDataType::Int: return 4;
    case ShaderDataType::Veci2: return 4 * 2;
    case ShaderDataType::Veci3: return 4 * 3;
    case ShaderDataType::Veci4: return 4 * 4;
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

    BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
        : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {
    }

    void SetOffset(size_t totalOffset) {
        offset = totalOffset;
    }

    uint32_t GetComponentCount() const {
        switch (type) {
        case ShaderDataType::None: return 0;
        case ShaderDataType::Float: return 1;
        case ShaderDataType::Vecf2: return 2;
        case ShaderDataType::Vecf3: return 3;
        case ShaderDataType::Vecf4: return 4;
        case ShaderDataType::Mat3: return 3; // 3* float3
        case ShaderDataType::Mat4: return 4; // 4* float4
        case ShaderDataType::Int: return 1;
        case ShaderDataType::Veci2: return 2;
        case ShaderDataType::Veci3: return 3;
        case ShaderDataType::Veci4: return 4;
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

    void push_back(const BufferElement& element) {
        elements_.push_back(std::move(element));
    }

private:
    void
    CalculateOffsetsAndStride() {
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


// describe layout in BufferLayout
// SetData() api for set actually data to layout
// Bind() api for Bind VBO State
class VertexBuffer {
public:
    VertexBuffer(uint32_t size, bool interleaved = false);
    VertexBuffer(float* vertices, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    template <typename T>
    VertexBuffer& SetData(const std::vector<T>& vec) {
        if (interleaved_)
            spdlog::error("[OpenGL] Already use SOA(interleaved) data model");
        glBindBuffer(GL_ARRAY_BUFFER, vboID_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vec.size() * sizeof(T), vec.data()); // [0, 0+size]
        return *this;
    }

    // void SetData(const void* data, uint32_t size);
    template <typename T>
    VertexBuffer& SetSubData(BufferElement&& element, const std::vector<T>& vec) {
        if (!interleaved_)
            spdlog::error("[OpenGL] Already use AOS(not interleaved) data model");

        element.SetOffset(totalBytes);
        layout_.push_back(element);
        glBindBuffer(GL_ARRAY_BUFFER, vboID_);
        // `glBufferSubData just updates a buffer object's data store
        // but doesn't creates a buffer object's data store.
        // You need to create the buffer object's data store with `glBufferData,
        // after that you can update the data with `glBufferSubData.
        glBufferSubData(GL_ARRAY_BUFFER, totalBytes, vec.size() * sizeof(T), vec.data());
        spdlog::info("[OpenGL] &{} Offset: {} Size: {}", __func__, totalBytes, vec.size() * sizeof(T));
        totalBytes += vec.size() * sizeof(T);
        return *this;
    }

    const BufferLayout& GetLayout() const { return layout_; }
    VertexBuffer&       SetLayout(const BufferLayout& layout) {
        layout_ = layout;
        return *this;
    }

    uint32_t GetBytes() const { return totalBytes; };
    uint32_t GetSize() const { return totalBytes / layout_.GetStride(); };

private:
    uint32_t     vboID_;
    BufferLayout layout_;
    uint32_t     totalBytes   = 0;
    bool         interleaved_ = 0;
};


// describe layout in BufferLayout
// SetData() api for set actually data to layout
// Bind() api for Bind EBO State
class IndexBuffer {
public:
    IndexBuffer(uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t GetCount() const { return count_; }

    static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);

private:
    uint32_t eboID_;
    uint32_t count_;
};

} // namespace EG::Utils
