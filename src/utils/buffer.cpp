#include "utils/buffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace EG::Utils {

VertexBuffer::VertexBuffer(uint32_t size, bool interleaved)
    : interleaved_(interleaved), layout_() {
    glCreateBuffers(1, &vboID_);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    if (interleaved)
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
    : interleaved_(0) {

    glCreateBuffers(1, &vboID_);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &vboID_);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
    glCreateBuffers(1, &eboID_);

    // `GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
    // Binding with `GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
    glBindBuffer(GL_ARRAY_BUFFER, eboID_);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &eboID_);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID_);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace EG::Utils