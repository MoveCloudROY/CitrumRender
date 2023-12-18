#include "core/Application.h"
#include "core/imgui/ImGuiLayer.h"
#include "core/imgui/theme.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>


namespace EG {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {
}

void ImGuiLayer::OnAttach() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking Windows
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi Viewports

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    Application& app    = Application::Get();
    GLFWwindow*  window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    // HiDPI
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    auto meanscale = (xscale + yscale) / 2.f;
    if (meanscale <= 0.0F)
        meanscale = 1.0F;

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        // style.WindowRounding              = 0.0f;
        // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        EG::Core::theme::SetTheme();
        style.ScaleAllSizes(meanscale);
    }
    // Basic Latin, Extended Latin
    io.Fonts->AddFontFromFileTTF("../src/assets/fonts/NotoSans-Medium.ttf", 16 * meanscale, nullptr, io.Fonts->GetGlyphRangesDefault());

    // Default + Selection of 2500 Ideographs used by Simplified Chinese
    io.Fonts->AddFontFromFileTTF("../src/assets/fonts/xihei-screen.ttf", 16 * meanscale, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
    // io.Fonts->AddFontFromFileTTF("font.ttf", 13, nullptr, io.Fonts->GetGlyphRangesJapanese());

    io.FontGlobalScale = meanscale;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}


void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& e) {
    if (blockEvents_) {
        ImGuiIO& io = ImGui::GetIO();
        e.handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        e.handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    ImGuiIO&     io  = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize   = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace EG