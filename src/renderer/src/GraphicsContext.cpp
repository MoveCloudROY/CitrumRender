#include "renderer/GraphicsContext.h"
#include "renderer/RHI.h"
#include <platform/graphics/opengl/OpenGLContext.h>

#include <core/Log.h>

namespace EG {

std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window) {
    switch (RHI::GetAPI()) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
    }

    return nullptr;
}

} // namespace EG