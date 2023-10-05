#include "model/mesh.h"
#include <spdlog/spdlog.h>
using namespace runtime;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
    : vertices_{std::move(vertices)}
    , indices_{std::move(indices)}
    , textures_{std::move(textures)} {
    InitMesh();
}

Mesh::Mesh(const Mesh& rhs)
    : vertices_{rhs.vertices_}
    , indices_{rhs.indices_}
    , textures_{rhs.textures_} {
    InitMesh();
}

Mesh::Mesh(Mesh&& rhs)
    : vertices_{std::move(rhs.vertices_)}
    , indices_{std::move(rhs.indices_)}
    , textures_{std::move(rhs.textures_)} {
    InitMesh();
}

Mesh& Mesh::operator=(const Mesh& rhs) {
    vertices_ = rhs.vertices_;
    indices_  = rhs.indices_;
    textures_ = rhs.textures_;
    InitMesh();

    return *this;
}
Mesh& Mesh::operator=(Mesh&& rhs) {
    vertices_ = std::move(rhs.vertices_);
    indices_  = std::move(rhs.indices_);
    textures_ = std::move(rhs.textures_);
    InitMesh();

    return *this;
}

void Mesh::Draw(Utils::Shader shader) {
#define TEXTYPE_CREATE_NR(name) \
    size_t name##_nr = 0;

#define DO_WORKS(name)                                                                          \
    {                                                                                           \
        shader.setInt(std::string{"material."} + #name + std::to_string(name##_nr).c_str(), i); \
        name##_nr++;                                                                            \
        break;                                                                                  \
    }

    TEXTYPE_X(TEXTYPE_CREATE_NR);

    for (size_t i = 0; i < textures_.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        switch (textures_[i].type) {
            TEXTYPE_X(TEXTYPE_CREATE_SWITCH, DO_WORKS);
        }
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
#undef DO_WORKS
#undef TEXTYPE_CREATE_NR
}


void Mesh::InitMesh() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), indices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

    glBindVertexArray(0);
}