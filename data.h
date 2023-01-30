#pragma once

#include <GL/glew.h>
#include <vector>

namespace Utils {
    unsigned int createVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

}