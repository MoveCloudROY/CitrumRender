#include "platform/graphics/opengl/OpenGLAPI.h"
#include "core/Log.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace EG {

void OpenGLMessageCallback(
    unsigned    source,
    unsigned    type,
    unsigned    id,
    unsigned    severity,
    int         length,
    const char* message,
    const void* userParam
) {
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: EG_CORE_CRITICAL(message); return;
    case GL_DEBUG_SEVERITY_MEDIUM: EG_CORE_ERROR(message); return;
    case GL_DEBUG_SEVERITY_LOW: EG_CORE_WARN(message); return;
    case GL_DEBUG_SEVERITY_NOTIFICATION: EG_CORE_TRACE(message); return;
    }

    EG_CORE_ASSERT(false, "[RHI] Unknown severity level!");
}

void OpenGLAPI::Init() {

#ifdef EG_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}
void OpenGLAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void OpenGLAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace EG