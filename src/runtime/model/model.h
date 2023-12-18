#ifndef EG_MODEL_H__
#define EG_MODEL_H__
#include "mesh.h"

#include <unordered_set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace EG::runtime {

class Model {
public:
    Model(const std::string& path);
    ~Model() = default;

    void Draw(Utils::Shader shader);

private:
    void                 LoadModel(const std::string& path);
    void                 ProcessNode(aiNode* node, const aiScene* scene);
    Mesh                 ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TexType type_enum);

    // use transparent verison equal_to<> and hash
    // for heterogeneous comparation
    std::unordered_set<Texture, TextureHash, std::equal_to<>> loaded_textures_;
    std::vector<Mesh>                                         meshes_;
    std::string                                               directory_;
};

} // namespace EG::runtime

#endif /* EG_MODEL_H__ */