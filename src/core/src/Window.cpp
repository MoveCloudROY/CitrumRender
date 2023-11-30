#include "core/Window.h"
#include "platform/os/general/GlfwWindow.h"
#include <memory>

namespace EG {
std::unique_ptr<Window> Window::Create(const WindowProps& props) {
    // general
    return std::make_unique<GlfwWindow>(props);
}

} // namespace EG