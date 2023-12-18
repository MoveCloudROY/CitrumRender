#ifndef EG_CORE_APPLICATION_H__
#define EG_CORE_APPLICATION_H__

#include "Layer.h"
#include "LayerStack.h"
#include "imgui/ImGuiLayer.h"
#include "Window.h"

#include "events/ApplicationEvents.h"
#include <memory>
#include <mutex>

namespace EG {

struct ApplicationCommandLineArgs {
    int    count = 0;
    char** args  = nullptr;

    const char* operator[](int index) const {
        return args[index];
    }
};

struct ApplicationProps {
    std::string                name = "EG";
    std::string                workingDirectory;
    ApplicationCommandLineArgs commandLineArgs;
};

class Application {
public:
    Application(const ApplicationProps& props);
    virtual ~Application() = default;

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    void SubmitToMainThread(const std::function<void()>& function);


    Window&                 GetWindow() { return *window_; }
    ImGuiLayer*             GetImGuiLayer() { return imguiLayer_; }
    static Application&     Get() { return *instance_; }
    const ApplicationProps& GetSpecification() const { return props_; }


private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    void ExecuteMainThreadQueue();

    ApplicationProps props_;
    ImGuiLayer*      imguiLayer_;

    static inline Application* instance_ = nullptr;
    std::unique_ptr<Window>    window_;

    bool  running_       = true;
    bool  minimized_     = false;
    float lastFrameTime_ = 0.0f;

    LayerStack layerStack_;

    std::vector<std::function<void()>> mainThreadQueue_;
    std::mutex                         mainThreadQueueMutex_;
};

// To be defined in CLIENT
Application* CreateApplication(ApplicationCommandLineArgs args);

} // namespace EG


#endif