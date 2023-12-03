#ifndef EG_DEFINEDATA_H__
#define EG_DEFINEDATA_H__
#include <vector>

namespace EG::Utils {

// clang-format off
static std::vector<float> skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

// clang-format off

static std::vector<float> cubeVertices = {
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f,  0.5f, -0.5f, 
    0.5f,  0.5f, -0.5f,
    0.5f,  -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f, 
    -0.5f, 0.5f,  -0.5f, 
    -0.5f,  -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, 0.5f, 
    -0.5f,  0.5f,  0.5f, 

    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f,  0.5f,  
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
};
static std::vector<float> cubeNormals = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,   
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f, 
    0.0f,  0.0f, 1.0f, 
    0.0f,  0.0f, 1.0f, 
    0.0f,  0.0f, 1.0f, 
    0.0f,  0.0f, 1.0f, 

    -1.0f,  0.0f,  0.0f, 
    -1.0f,  0.0f,  0.0f, 
    -1.0f,  0.0f,  0.0f, 
    -1.0f,  0.0f,  0.0f, 
    -1.0f,  0.0f,  0.0f, 
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,    
    1.0f,  0.0f,  0.0f, 
    1.0f,  0.0f,  0.0f, 
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,    
    1.0f,  0.0f,  0.0f, 

    0.0f, -1.0f,  0.0f, 
    0.0f, -1.0f,  0.0f, 
    0.0f, -1.0f,  0.0f, 
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f, 
    0.0f, -1.0f,  0.0f, 

    0.0f,  1.0f,  0.0f, 
    0.0f,  1.0f,  0.0f, 
    0.0f,  1.0f,  0.0f, 
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f, 
    0.0f,  1.0f,  0.0f,
};

static std::vector<float> cubeUVs = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,

    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
static std::vector<float> windowVerticesPos = { 
    -1.0f,  1.0f,  
    -1.0f, -1.0f,  
    1.0f, -1.0f,  

    -1.0f,  1.0f,
    1.0f, -1.0f, 
    1.0f,  1.0f
};

static std::vector<float> windowVerticesUV = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};



// clang-format on

} // namespace EG::Utils

#endif /* EG_DEFINEDATA_H__ */