#pragma once
#include "imgui/imgui.h"

void guiStart();
void SetupFonts();
void setImguiStyle();
void drawLines(ImVec2& end, ImColor color);
void drawCircle(ImVec2& end, ImColor color);
void drawFovCircle(float radius);