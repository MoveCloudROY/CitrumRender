#pragma once

#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include <memory>

namespace EG {

struct ApplicationCommandLineArgs {
    int    count = 0;
    char** args  = nullptr;

    const char* operator[](int index) const {
        return args[index];
    }
};

struct ApplicationSpecification {
    std::string                name = "EG";
    std::string                workingDirectory;
    ApplicationCommandLineArgs CommandLineArgs;
};

class Application {
public:
    Application(const ApplicationSpecification& specification);
    virtual ~Application();

    void Run();

    void onEvent();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    Window& GetWindow() { return *m_Window; }

private:
    // bool OnWindowClose(WindowCloseEvent& e);
    static Application*     m_Instance;
    std::unique_ptr<Window> m_Window;

    bool       m_Running = true;
    LayerStack m_LayerStack;
};

// To be defined in CLIENT
Application* CreateApplication();

} // namespace EG