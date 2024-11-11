#include "gui.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>

#include "imgui/imgui.h"

#include "options.h"
#include "fonts.hpp"

inline ImFont* iconsFont = nullptr;
inline ImFont* largeFont = nullptr;

void setImguiStyle() {
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);

    colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_ButtonActive] = ImColor(255, 122, 0, 255);
    colors[ImGuiCol_ButtonHovered] = ImColor(255, 140, 0, 255);

    colors[ImGuiCol_SliderGrab] = ImColor(16, 24, 32, 255);
    colors[ImGuiCol_SliderGrabActive] = ImColor(16, 24, 32, 255);

    colors[ImGuiCol_CheckMark] = ImColor(0, 0, 0, 255);

    colors[ImGuiCol_FrameBg] = ImColor(255, 255, 255);
    colors[ImGuiCol_FrameBgActive] = ImColor(255, 255, 255);
    colors[ImGuiCol_FrameBgHovered] = ImColor(255, 255, 255);

    colors[ImGuiCol_PopupBg] = ImColor(255, 255, 255, 255);

    colors[ImGuiCol_WindowBg] = ImColor(16, 24, 32, 255);

    colors[ImGuiCol_TitleBg] = ImColor(16, 24, 32, 255);
    colors[ImGuiCol_TitleBgActive] = ImColor(16, 24, 32, 255);
    colors[ImGuiCol_TitleBgCollapsed] = ImColor(16, 24, 32, 255);

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.FramePadding = ImVec2(0, 0);
    style.WindowPadding = ImVec2(0, 0);
    style.ItemSpacing = ImVec2(0, 0);
    style.WindowRounding = 5.f;
    style.FrameRounding = 4.0f;
    style.GrabMinSize = 9.f;
    style.GrabRounding = 100.f;
}

void SetupFonts() {
    static const ImWchar iconRanges[]{ ICON_MIN_FA, ICON_MAX_FA, 0 };

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    static const ImWchar* glyphRanges = io.Fonts->GetGlyphRangesCyrillic();

    ImFont* fontLarge = io.Fonts->AddFontFromMemoryTTF((void*)custom_font, sizeof(custom_font), 19.f, nullptr, glyphRanges);

    largeFont = io.Fonts->AddFontFromMemoryTTF((void*)custom_font, sizeof(custom_font), 30.f);

    io.Fonts->AddFontFromMemoryTTF((void*)custom_font, sizeof(custom_font), 19.f);

    ImFontConfig iconsConfig;

    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.OversampleH = 3;
    iconsConfig.OversampleV = 3;

    iconsFont = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 22.0f, &iconsConfig, iconRanges);
}

bool ToggleButton(const char* str_id, bool* v, float button_width = 0.0f, float button_height = 0.0f) {
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    if (button_width == 0.0f) {
        button_width = ImGui::GetFrameHeight() * 1.55f;
    }
    if (button_height == 0.0f) {
        button_height = ImGui::GetFrameHeight();
    }

    float radius = button_height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(button_width, button_height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    ImU32 col_bg = *v ? IM_COL32(255, 140, 0, 255) : IM_COL32(255, 255, 255, 255);
    ImU32 circle_bg = *v ? IM_COL32(255, 255, 255, 255) : IM_COL32(16, 24, 32, 255);

    draw_list->AddRectFilled(p, ImVec2(p.x + button_width, p.y + button_height), col_bg, button_height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(*v ? (p.x + button_width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, circle_bg);

    return *v;
}

void ShowColorPickerButton(ImVec4* color, int id) {
    ImGui::PushID(id);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 1));

    if (ImGui::ColorButton("ColorButton", *color)) {
        ImGui::OpenPopup("ColorPickerPopup");
    }

    if (ImGui::BeginPopup("ColorPickerPopup")) {
        ImGui::ColorPicker4("##picker", (float*)color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

ImVec2 menuSize(220, 525);
ImVec2 paddings(0.f, 15.f);
float leftMargin = 10.f;

static std::string keyName = "Press a key";
bool waitingForEspKey = false;

bool keyPressed = false;

const char* aimbotButtons[] = { "Right", "Left" };

void drawLines(ImVec2& end, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(options::monitorX / 2.0f, options::monitorY), end, color, 0.7f);
}

void drawCircle(ImVec2& end, ImColor color) {
    float radius = options::circleRadius;

    ImGui::GetBackgroundDrawList()->AddCircleFilled(end, radius, color);
}

void drawFovCircle(float radius) {
    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2{ options::monitorX / 2.f, options::monitorY / 2.f }, radius, ImColor(255, 255, 255, 255), 50, 1.f);
}

void guiStart() {
    ImGui::Begin("Virelex");
    ImGui::SetWindowSize(menuSize, ImGuiCond_Always);

    ImGui::Indent(leftMargin);
    ImGui::Dummy(paddings);

    ToggleButton("#esp", &menu::bEspEnabled, 30, 20);
    ImGui::SameLine();
    ImGui::Text("  ESP ");

    ImGui::Dummy(paddings);

    ToggleButton("#lines", &menu::bLinesEnabled, 30, 20);
    ImGui::SameLine();
    ImGui::Text("  Lines ");
    ImGui::SameLine();
    ShowColorPickerButton(&options::linesColor, 10);

    ImGui::Dummy(paddings);

    ToggleButton("#points", &menu::bPointsEnabled, 30, 20);
    ImGui::SameLine();
    ImGui::Text("  Points ");
    ImGui::SameLine();
    ShowColorPickerButton(&options::pointsColor, 20);

    ImGui::Dummy(paddings);

    ImGui::Text("Points size");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
    ImGui::SliderFloat(" ", &options::circleRadius, 1.0f, 20.0f, "%.3f");
    ImGui::PopStyleColor();

    ImGui::Dummy(paddings);
    ImGui::Separator();
    ImGui::Dummy(paddings);

    ToggleButton("#aim", &menu::bAimbotEnabled, 30, 20);
    ImGui::SameLine();
    ImGui::Text("  Aimbot");

    ImGui::Dummy(paddings);

    ImGui::Text("Activate aimbot");
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 140.f / 255, 0, 1.f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
    ImGui::Combo("Select Option", &options::selectedAimbot, aimbotButtons, IM_ARRAYSIZE(aimbotButtons));
    ImGui::PopStyleColor(2);

    ImGui::Dummy(paddings);

    ImGui::Text("Smoothness");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
    ImGui::SliderFloat("  ", &options::smoothness, 1.0f, 3.5f, "%.3f");
    ImGui::PopStyleColor(2);

    ImGui::Dummy(paddings);

    ToggleButton("#fov", &menu::bFOV, 30, 20);
    ImGui::SameLine();
    ImGui::Text("  FOV");

    ImGui::Dummy(paddings);

    ImGui::Text("FOV");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(16.f / 255.f, 24.f / 255.f, 32.f / 255.f, 1.f));
    ImGui::SliderFloat("   ", &options::radiusFOV, 1.0f, 300.0f, "%.3f");
    ImGui::PopStyleColor(2);

    ImGui::Dummy(paddings);
    ImGui::Separator();
    ImGui::Dummy(paddings);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 140.f / 255, 0, 1.f));
    if (ImGui::Button("ESP key", ImVec2(130, 25))) {
        waitingForEspKey = true;
    }

    ImGui::Dummy(paddings);

    if (waitingForEspKey) {
        ImGui::Text("Press key...");
        ImGui::SetWindowSize({ menuSize.x, menuSize.y + 10 }, ImGuiCond_Always);

        for (int key = 0; key < 256; ++key) {
            if (GetAsyncKeyState(key) & 0x8000) {
                menu::bToggleESP = key;
                waitingForEspKey = false;
                break;
            }
        }
    }
    else {
        ImGui::Text("Key: % s", ImGui::GetKeyName(static_cast<ImGuiKey>(menu::bToggleESP)));
    }

    ImGui::PopStyleColor();

    ImGui::End();
}