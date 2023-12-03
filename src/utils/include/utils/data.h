#ifndef EG_DATA_H__
#define EG_DATA_H__

#include <GL/glew.h>
#include <vector>

namespace EG::Utils {
unsigned int createVAO(const std::vector<float>& vertices);
void         createEBO(const std::vector<unsigned int>& indices);
} // namespace EG::Utils

#endif /* EG_DATA_H__ */