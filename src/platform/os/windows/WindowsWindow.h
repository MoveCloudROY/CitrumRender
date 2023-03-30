#pragma once

#include "editor/Window.h"

namespace EG {

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);

private:
    virtual void Init(const WindowsWindow& props);
    virtual void Shutdown();

};

}