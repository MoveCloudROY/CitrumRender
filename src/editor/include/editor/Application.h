#pragma once

#include "Layer.h"

#include <memory>

namespace EG {

class Application {
public:
    Application();
    virtual ~Application();

    void Run();

    void onEvent();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

private:
    // bool OnWindowClose(WindowCloseEvent& e);

    // static std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};



}