#include "renderer/VertexArray.h"
#include "renderer/RHI.h"

#include <platform/graphics/opengl/OpenGLVertexArray.h>
#include <core/Log.h>

namespace EG {

std::shared_ptr<VertexArray> VertexArray::Create() {
    switch (RHI::GetAPI()) {
    case RHI::API::None: EG_CORE_ASSERT(false, "[RHI] Unknown Graphics API!"); return nullptr;
    case RHI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
    }
    return nullptr;
}

} // namespace EG