#pragma once
#include "../Layer.h"
#include <imgui.h>

namespace EG {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    // ImGui Layer will be initialized when push to layer stack
    // in Application::PushLayer/Application::PushOverLayer
    virtual void OnAttach() override;
    // ImGui Layer End When
    virtual void OnDetach() override;
    virtual void OnEvent(Event& e) override;

    // Declare ImGui Section
    void Begin();
    void End();

    // wait for input
    void BlockEvents(bool block) { blockEvents_ = block; }

private:
    bool blockEvents_ = true;
};
} // namespace EG