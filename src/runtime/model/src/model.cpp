#include "model/model.h"

#include <texture.h>

#include <spdlog/spdlog.h>
#include <stb_image.h>


using namespace runtime;

Model::Model(const std::string& path) {
    LoadModel(path);
}

void Model::Draw(Utils::Shader shader) {
    for (auto&& i : meshes_) {
        i.Draw(shader);
    }
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

    if (nullptr == scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("[ASSIMP] {} {} {} {}", __FILE_NAME__, __LINE__, __func__, importer.GetErrorString());
        return;
    }
    directory_ = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}
void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        auto p_mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(std::move(ProcessMesh(p_mesh, scene)));
    }
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture>  textures;

    // solve vertices
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {

        glm::vec2 texcoords;
        if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
            texcoords = glm::vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        else
            texcoords = glm::vec2{0.0f, 0.0f};
        vertices.emplace_back(
            // position
            glm::vec3{
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z,
            },
            // normal
            glm::vec3{
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z,
            },
            // texcoords
            std::move(texcoords)
        );
    }
    // solve indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        // get each triangle's index
        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // solve texture
    if (mesh->mMaterialIndex >= 0) {
        auto material     = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps  = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::diffuse);
        auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TexType::diffuse);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return {std::move(vertices), std::move(indices), std::move(textures)};
}
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TexType type_enum) {
    std::vector<Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString name;
        mat->GetTexture(type, i, &name);
        auto it = loaded_textures_.find(name.C_Str());
        if (it != loaded_textures_.end()) {
            textures.push_back(*it);
        } else {
            Texture tmp{
                Utils::TextureBuilder{}.build(directory_ + '/' + name.C_Str(), false),
                type_enum,
                name};
            textures.push_back(tmp);
            loaded_textures_.insert(std::move(tmp));
        }
    }
    return textures;
}