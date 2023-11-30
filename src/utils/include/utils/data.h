#ifndef EG_DATA_H__
#define EG_DATA_H__

#include <GL/glew.h>
#include <vector>

namespace Utils {
    unsigned int createVAO(const std::vector<float>& vertices);
    void createEBO(const std::vector<unsigned int>& indices);
}

#endif /* EG_DATA_H__ */