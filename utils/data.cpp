
#include "include/data.h"

namespace Utils {

unsigned int createVAO(
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices
) {
    // 创建 VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO 创建与数据绑定
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    // 创建 EBO(IBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 同时将状态保存到 VAO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    // 如何从VBO解析顶点属性,并将状态保存到 VAO
    // '0' => Corresponding `location` in vertex shader Attribute value
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 以顶点属性位置值作为参数，启用顶点属性
    glEnableVertexAttribArray(0);

    return VAO;
}

} /* NAMESPACE `Utils` END */