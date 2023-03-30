#include "editor/imgui/ImGuiLayer.h"

namespace EG {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") 
{
    
}

void ImGuiLayer::onAttach() {
   // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking Windows
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi Viewports

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    // // Setup Platform/Renderer backends
    // ImGui_ImplGlfw_InitForOpenGL(window, true);
    // ImGui_ImplOpenGL3_Init("#version 330");
}


void ImGuiLayer::onDetach() {
    
}

void ImGuiLayer::onUpdate() {
    
}

void ImGuiLayer::Begin() {

}

void ImGuiLayer::End() {

}

}