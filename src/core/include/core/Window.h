#pragma once

#include "events/Events.h"
#include <string>
#include <functional>
#include <memory>

namespace EG {

struct WindowProps {
    std::string title  = "EG Default Window";
    uint32_t    width  = 1920;
    uint32_t    height = 1080;
};

// Window Interface
class Window {
public:
    using EventCallBackFunc = std::function<void(Event&)>;
    virtual ~Window();

    virtual uint32_t GetWidth() const  = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void OnUpdate() = 0;

    // Provide function to deal with the Window Event
    virtual void SetEventCallback(const EventCallBackFunc& callback) = 0;
    virtual void SetVSync(bool enable)                               = 0;
    virtual bool IsVSync() const                                     = 0;

    virtual void* GetNativeWindow() const = 0;

    static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};

} // namespace EG