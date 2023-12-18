#pragma once

#include "core/Window.h"
#include "renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace EG {

class GlfwWindow : public Window {
public:
    GlfwWindow(const WindowProps& props);
    virtual ~GlfwWindow() override;

    virtual uint32_t GetWidth() const override { return data_.width; };
    virtual uint32_t GetHeight() const override { return data_.height; };

    virtual void OnUpdate() override;

    virtual void SetEventCallback(const EventCallBackFunc& callback) override {
        data_.EventCallback = callback;
    };

    virtual void SetVSync(bool enable) override;
    virtual bool IsVSync() const override;

    virtual void* GetNativeWindow() const override { return window_; };

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

private:
    GLFWwindow*                      window_;
    std::unique_ptr<GraphicsContext> context_;

    struct WindowData {
        std::string  title;
        unsigned int width, height;
        bool         vSync;

        EventCallBackFunc EventCallback;
    };

    WindowData data_;
};

} // namespace EG