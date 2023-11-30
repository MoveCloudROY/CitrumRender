#include "platform/os/general/GlfwWindow.h"

#include "core/events/ApplicationEvents.h"
#include "core/events/MouseEvents.h"
#include "core/events/KeyEvents.h"

#include <renderer/RHI.h>

namespace EG {

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description) {
    // EG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

GlfwWindow::GlfwWindow(const WindowProps& props) {

    Init(props);
}

GlfwWindow::~GlfwWindow() {
    Shutdown();
}

void GlfwWindow::Init(const WindowProps& props) {

    data_.title  = props.title;
    data_.width  = props.width;
    data_.height = props.height;


    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
#if defined(EG_DEBUG)
        if (RHI::GetAPI() == RHI::API::OpenGL)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        window_ = glfwCreateWindow((int)props.width, (int)props.height, data_.title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;
    }

    context_ = GraphicsContext::Create(window_);
    context_->Init();

    glfwSetWindowUserPointer(window_, &data_);
    SetVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width       = width;
        data.height      = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
        WindowData&      data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(key, true);
            data.EventCallback(event);
            break;
        }
        }
    });

    glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });
}

void GlfwWindow::Shutdown() {

    glfwDestroyWindow(window_);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void GlfwWindow::OnUpdate() {

    glfwPollEvents();
    context_->SwapBuffers();
}

void GlfwWindow::SetVSync(bool enabled) {

    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    data_.vSync = enabled;
}

bool GlfwWindow::IsVSync() const {
    return data_.vSync;
}

} // namespace EG
