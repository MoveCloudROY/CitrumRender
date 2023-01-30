#ifndef MY_DATA_H__
#define MY_DATA_H__

#include <GL/glew.h>
#include <vector>

namespace Utils {
    unsigned int createVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

}

#endif /* MY_DATA_H__ */