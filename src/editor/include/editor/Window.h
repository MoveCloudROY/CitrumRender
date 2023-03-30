#pragma once

#include "events/Event.h"
#include <string>
#include <functional>
#include <memory>

namespace EG {

struct WindowProps {
    std::string   title;
    std::uint32_t width;
    std::uint32_t height;
    WindowProps(
        const std::string& title  = "EG Default Window",
        uint32_t           width  = 1920,
        uint32_t           height = 1080
    )
        : title(title), width(width), height(height) {}
};

// Window Interface
class Window {
public:
    using EventCallBackFunc = std::function<void(Event&)>;
    ~Window() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void onUpdate() = 0;
    
    virtual void SetEventCallBack(const EventCallBackFunc& callback) = 0;
    virtual void setVSync(bool enable) = 0;
    virtual void IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};

} // namespace EG