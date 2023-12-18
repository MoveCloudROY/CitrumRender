#ifndef EG_IMGUI_THEME_H__
#define EG_IMGUI_THEME_H__

#include <imgui.h>
#include <string>
#include <string_view>
#include <optional>

namespace EG::Core::theme {

using Color = ImVec4;

struct Theme {

    std::string_view name; // if you want to give a name to the theme

    std::optional<Color> text;                        // global text color
    std::optional<Color> window_bg;                   // ImGuiCol_WindowBg & ImGuiCol_ChildBg
    std::optional<Color> border;                      // ImGuiCol_Border
    std::optional<Color> border_shadow;               // ImGuiCol_BorderShadow
    std::optional<Color> button;                      // ImGuiCol_Button
    std::optional<Color> button_hovered;              // ImGuiCol_ButtonHovered
    std::optional<Color> button_active;               // ImGuiCol_ButtonActive
    std::optional<Color> frame_bg;                    // ImGuiCol_FrameBg
    std::optional<Color> frame_bg_hovered;            // ImGuiCol_FrameBgHovered
    std::optional<Color> frame_bg_active;             // ImGuiCol_FrameBgActive
    std::optional<Color> text_selected_bg;            // ImGuiCol_TextSelectedBg, for text input field
    std::optional<Color> check_mark;                  // ImGuiCol_CheckMark
    std::optional<Color> title_bg;                    // ImGuiCol_TitleBg
    std::optional<Color> title_bg_active;             // ImGuiCol_TitleBgActive
    std::optional<Color> title_bg_collapsed;          // ImGuiCol_TitleBgCollapsed
    std::optional<Color> message_panel;               // logging panel
    std::optional<Color> auto_complete_selected;      // left-most text in the autocompletion OSD
    std::optional<Color> auto_complete_non_selected;  // every text but the left most in the autocompletion OSD
    std::optional<Color> auto_complete_separator;     // color for the separator in the autocompletion OSD
    std::optional<Color> cmd_backlog;                 // color for message type user_input
    std::optional<Color> cmd_history_completed;       // color for message type cmd_history_completion
    std::optional<Color> log_level_drop_down_list_bg; // ImGuiCol_PopupBg
    std::optional<Color> log_level_active;            // ImGuiCol_HeaderActive
    std::optional<Color> log_level_hovered;           // ImGuiCol_HeaderHovered
    std::optional<Color> log_level_selected;          // ImGuiCol_Header
    std::optional<Color> scrollbar_bg;                // ImGuiCol_ScrollbarBg
    std::optional<Color> scrollbar_grab;              // ImGuiCol_ScrollbarGrab
    std::optional<Color> scrollbar_grab_active;       // ImGuiCol_ScrollbarGrabActive
    std::optional<Color> scrollbar_grab_hovered;      // ImGuiCol_ScrollbarGrabHovered
    std::optional<Color> filter_hint;                 // ImGuiCol_TextDisabled
    std::optional<Color> filter_text;                 // user input in log filter
    std::optional<Color> matching_text;               // text matching the log filter

    std::optional<float> tab_rounding;
    std::optional<float> scrollbar_rounding;
    std::optional<float> window_rounding;
    std::optional<float> grab_rounding;
    std::optional<float> frame_rounding;
    std::optional<float> popup_rounding;
    std::optional<float> child_rounding;
};


inline void SetTheme() {

    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg]  = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Border
    colors[ImGuiCol_Border]       = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

    // Text
    colors[ImGuiCol_Text]         = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
    colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

    // Headers
    colors[ImGuiCol_Header]        = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_HeaderActive]  = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button]        = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ButtonActive]  = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_CheckMark]     = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

    // Popups
    colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

    // Sliderx
    colors[ImGuiCol_SliderGrab]       = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
    colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

    // Frame BG
    colors[ImGuiCol_FrameBg]        = ImVec4{0.13f, 0.13, 0.17, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_FrameBgActive]  = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Tabsx
    colors[ImGuiCol_Tab]                = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabHovered]         = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
    colors[ImGuiCol_TabActive]          = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
    colors[ImGuiCol_TabUnfocused]       = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg]          = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgActive]    = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg]          = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

    // Seperatorx
    colors[ImGuiCol_Separator]        = ImVec4{0.44f, 0.37f, 0.61f, 0.2f};
    colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.2f};
    colors[ImGuiCol_SeparatorActive]  = ImVec4{0.84f, 0.58f, 1.0f, 0.2f};

    // Resize Gripx
    colors[ImGuiCol_ResizeGrip]        = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
    colors[ImGuiCol_ResizeGripActive]  = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};

    // Dockingx
    colors[ImGuiCol_DockingPreview] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};

    auto& style             = ImGui::GetStyle();
    style.TabRounding       = 4;
    style.ScrollbarRounding = 9;
    style.WindowRounding    = 7;
    style.GrabRounding      = 3;
    style.FrameRounding     = 3;
    style.PopupRounding     = 4;
    style.ChildRounding     = 4;
}


} // namespace EG::Core::theme

#endif /* EG_IMGUI_THEME_H__ */