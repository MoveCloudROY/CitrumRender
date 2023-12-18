#include "renderer/RHI.h"

#include <platform/graphics/opengl/OpenGLAPI.h>

#include <core/Log.h>

namespace EG {

RHI::API RHI::api_ = RHI::API::OpenGL;

std::unique_ptr<RHI> RHI::Create() {
    switch (api_) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_unique<OpenGLAPI>();
    }
    return nullptr;
}

} // namespace EG