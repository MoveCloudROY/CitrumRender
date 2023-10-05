#ifndef EG_MESH_H__
#define EG_MESH_H__
#include <string>
#include <vector>
#include <functional>

#include <glm/glm.hpp>

#include <shader.h>
#include <assimp/types.h>

namespace runtime {

// clang-format off

// TexType X-macro
#define TEXTYPE_X(func, ...) \
    func(diffuse, ##__VA_ARGS__)        \
    func(specular, ##__VA_ARGS__)        \
    func(normal, ##__VA_ARGS__)       

#define TEXTYPE_CREATE_ENUM(name, ...) \
    name,

#define TEXTYPE_RETURN_STR(name) \
    return #name

#define TEXTYPE_CREATE_SWITCH(name, do_something) \
    case TexType::name: do_something(name); 

enum class TexType {
    TEXTYPE_X(TEXTYPE_CREATE_ENUM)
};

constexpr std::string Enum2String(TexType t) {
    switch (t) {
        TEXTYPE_X(TEXTYPE_CREATE_SWITCH, TEXTYPE_RETURN_STR);
        default: return "Not Exist";
    }
}

#undef TEXTYPE_CREATE_ENUM
#undef TEXTYPE_CREATE_STR
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

} // namespace runtime

#endif /* EG_MESH_H__ */