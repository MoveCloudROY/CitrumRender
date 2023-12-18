#include "utils/data.h"

namespace EG::Utils {

unsigned int createVAO(const std::vector<float>& vertices) {
    // 创建 VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO 创建与数据绑定
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(vertices[0]),
        vertices.data(),
        GL_STATIC_DRAW
    );
    return VAO;
}

void createEBO(const std::vector<unsigned int>& indices) {
    // 创建 EBO(IBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 同时将状态保存到 VAO
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW
    );
}

} // namespace EG::Utils