#include "core/Application.h"
#include "core/Timestep.h"
#include "core/events/Events.h"
#include <filesystem>
#include <mutex>

#include <platform/os/general/GlfwTools.h>

namespace EG {

Application::Application(const ApplicationProps& props)
    : props_(props) {

    instance_ = this;

    // Set working directory here
    if (!props_.workingDirectory.empty())
        std::filesystem::current_path(props_.workingDirectory);

    window_ = Window::Create(WindowProps(props_.name));
    window_->SetEventCallback(EG_MEMBER_FUNC_BIND(Application::OnEvent));

    imguiLayer_ = new ImGuiLayer();
    PushOverlay(imguiLayer_);
}


void Application::PushLayer(Layer* layer) {

    layerStack_.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {

    layerStack_.PushOverlay(layer);
    layer->OnAttach();
}


void Application::SubmitToMainThread(const std::function<void()>& function) {
    std::scoped_lock<std::mutex> lock(mainThreadQueueMutex_);

    mainThreadQueue_.emplace_back(function);
}

void Application::OnEvent(Event& e) {

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(EG_MEMBER_FUNC_BIND(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(EG_MEMBER_FUNC_BIND(Application::OnWindowResize));

    for (auto it = layerStack_.rbegin(); it != layerStack_.rend(); ++it) {
        if (e.handled)
            break;
        (*it)->OnEvent(e);
    }
}

void Application::Run() {

    while (running_) {

        float    time     = Time::GetTime();
        Timestep timestep = time - lastFrameTime_;
        lastFrameTime_    = time;
        ExecuteMainThreadQueue();

        if (!minimized_) {
            // Game tick (Logic & Render)
            {
                for (Layer* layer : layerStack_)
                    layer->OnUpdate(timestep);
            }
            // UI tick
            imguiLayer_->Begin();
            {
                for (Layer* layer : layerStack_)
                    layer->OnImGuiRender();
            }
            imguiLayer_->End();
        }
        // Window Update ( flash events and
        // Swap front and back buffers )
        window_->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    running_ = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {

    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        minimized_ = true;
        return false;
    }

    minimized_ = false;
    return false;
}

void Application::ExecuteMainThreadQueue() {
    std::scoped_lock<std::mutex> lock(mainThreadQueueMutex_);

    for (auto& func : mainThreadQueue_)
        func();

    mainThreadQueue_.clear();
}

} // namespace EG
