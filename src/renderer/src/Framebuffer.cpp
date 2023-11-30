#include "renderer/Framebuffer.h"

#include "renderer/RHI.h"

#include <core/Log.h>
#include <platform/graphics/opengl/OpenGLFramebuffer.h>

namespace EG {

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
    switch (RHI::GetAPI()) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
    }

    return nullptr;
}

} // namespace EG
