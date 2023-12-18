#ifndef EG_MESH_H__
#define EG_MESH_H__
#include <string>
#include <vector>
#include <functional>

#include <glm/glm.hpp>

#include <utils/shader.h>
#include <assimp/types.h>

namespace EG::runtime {

// clang-format off

// TexType X-macro
#define MATERIAL_TYPE_X(func, ...) \
    func(diffuse, ##__VA_ARGS__)        \
    func(specular, ##__VA_ARGS__)        \
    func(normal, ##__VA_ARGS__)       

#define MATERIAL_TYPE_CREATE_ENUM(name, ...) \
    name,

#define MATERIAL_TYPE_RETURN_STR(name) \
    return #name

#define MATERIAL_TYPE_CREATE_SWITCH(name, do_something) \
    case TexType::name: do_something(name); 

enum class TexType {
    MATERIAL_TYPE_X(MATERIAL_TYPE_CREATE_ENUM)
};

constexpr std::string Enum2String(TexType t) {
    switch (t) {
        MATERIAL_TYPE_X(MATERIAL_TYPE_CREATE_SWITCH, MATERIAL_TYPE_RETURN_STR);
        default: return "Not Exist";
    }
}

#undef MATERIAL_TYPE_CREATE_ENUM
#undef MATERIAL_TYPE_CREATE_STR
// clang-format on


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    uint32_t id;
    TexType  type;
    aiString name;

    bool operator==(const Texture& rhs) const {
        return (std::string_view{this->name.C_Str()} == std::string_view{rhs.name.C_Str()});
    }
    bool operator==(const char* rhs) const {
        return (std::string_view{this->name.C_Str()} == std::string_view{rhs});
    }
};

class TextureHash {
public:
    // Enable transparent functor for heterogeneous keys
    using is_transparent = void;

    size_t operator()(const Texture& rhs) const {
        return std::hash<std::string_view>()(rhs.name.C_Str());
    }
    size_t operator()(const char* rhs) const {
        return std::hash<std::string_view>()(rhs);
    }
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    ~Mesh() = default;
    Mesh(const Mesh& rhs);
    Mesh(Mesh&& rhs);
    Mesh& operator=(const Mesh& rhs);
    Mesh& operator=(Mesh&& rhs);


    void Draw(Utils::Shader shader);

    std::vector<Vertex>   vertices_;
    std::vector<uint32_t> indices_;
    std::vector<Texture>  textures_;

private:
    uint32_t vao_, vbo_, ebo_;
    void     InitMesh();
};

} // namespace EG::runtime

#endif /* EG_MESH_H__ */