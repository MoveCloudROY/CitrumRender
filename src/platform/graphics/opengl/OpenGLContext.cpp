#include "platform/graphics/opengl/OpenGLContext.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <core/Log.h>

namespace EG {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : windowHandle_(windowHandle) {
}

void OpenGLContext::Init() {

    /* Make the window's context current */
    glfwMakeContextCurrent(windowHandle_);

    if (glewInit() != GLEW_OK) {
        EG_CORE_ERROR("Failed to initialize GLEW");
    }

    EG_CORE_INFO("OpenGL Info:");
    EG_CORE_INFO("  Vendor:   {0}", (char*)glGetString(GL_VENDOR));
    EG_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
    EG_CORE_INFO("  Version:  {0}", (char*)glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(windowHandle_);
}

} // namespace EG
