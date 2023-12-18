#include "platform/os/general/GlfwTools.h"

#include <GLFW/glfw3.h>

namespace EG {
float Time::GetTime() {
    return glfwGetTime();
}


} // namespace EG