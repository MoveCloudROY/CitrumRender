#include "utils/cubemap.h"
#include "utils/texture.h"
#include "utils/data.h"
#include "utils/definedata.h"

namespace EG::Utils {


CubeMap::CubeMap(std::initializer_list<std::string> faces)
    : shader_("../src/shaders/vs/cubemap.vs", "../src/shaders/fs/cubemap.fs") {
    SetFaces(faces);
    cubeVaoId_ = createVAO(skyboxVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void CubeMap::SetFaces(std::initializer_list<std::string> faces) {
    auto builder = TextureBuilder(TextureType::TEXTURE_CUBE_MAP).SetProps();
    for (auto face : faces) {
        builder.Attach(face, false);
    }
    textureId_ = builder.Build();
}

void CubeMap::Draw(const glm::mat4& v, const glm::mat4& p) {
    // change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);
    shader_.use();
    shader_.setMatrix4f("view", v);
    shader_.setMatrix4f("projection", p);
    glBindVertexArray(cubeVaoId_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
}

} // namespace EG::Utils