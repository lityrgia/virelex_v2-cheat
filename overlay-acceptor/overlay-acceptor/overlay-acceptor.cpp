#include <windows.h>
#include <iostream>
#include "overlay.h"
#include "options.h"
#include "gui.h"
#include "sharedMemory.h"
#include "imgui/imgui.h"
#include <vector>
#include <cmath> 
#include "aimbot.h"

float calculateDistance(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
    return std::sqrt(std::pow(p2.first - p1.first, 2) + std::pow(p2.second - p1.second, 2));
}

int main() {
    system("title Virelex output");

    options::monitorX = (float)GetSystemMetrics(SM_CXSCREEN);
    options::monitorY = (float)GetSystemMetrics(SM_CYSCREEN);

    printf("[cheat]: Connecting to shared data...\n");
    while (!connectSharedMemory()) {
        Sleep(1000);
    }
    printf("[cheat]: Connected\n");

    if (overlay.CreateOverlay()) {
        printf("[cheat]: Succesfly created overlay\n");
    }
    else {
        printf("[cheat]: Failed to create overlay\n");
        return 0;
    }
 
    ImGui::CreateContext();
    overlay.imguiInit();
    setImguiStyle();
    SetupFonts();

    printf("[cheat]: Starting main loop\n\n");

    std::vector<std::pair<float, float>> coordsArray;

    MSG msg;
    while (overlay.done == false)
    {
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                overlay.done = true;
                break;
            }
        }

        if (overlay.done == true || GetAsyncKeyState(menu::bEndButton) & 1) break;

        bool isAimbotEnabled = (GetAsyncKeyState(options::selectedAimbot == 0 ? VK_RBUTTON : VK_LBUTTON) & 0x8000) != 0;
        bool isMenuVisible = (GetAsyncKeyState(menu::bToggleButton) & 1) != 0;

        if ((GetAsyncKeyState(menu::bToggleESP) & 1) != 0) menu::bEspEnabled = !menu::bEspEnabled;

        if (isMenuVisible)  menu::bMenuVisible = !menu::bMenuVisible;

        overlay.MakeNewFrame();

        ImVec2 targetScreenPosFin{ 0,0 };
        int closestToCrosshair{ 999999 };
        float fov{ 300 };

        getEntitiesArray(coordsArray);
        if (coordsArray.empty()) goto NO_COORDS;
 
        for (auto& pair : coordsArray) {
            ImVec2 coords{ (float)pair.first, (float)pair.second };

            if (coords.x == 0 || coords.y == 0) continue;

            if (menu::bEspEnabled) {
                if (menu::bLinesEnabled) {
                    drawLines(coords, options::linesColor);
                }
                if (menu::bPointsEnabled) {
                    drawCircle(coords, options::pointsColor);
                }
            }

            if (menu::bAimbotEnabled) {
                
                ImVec2 targetScreenPos = coords;
                float distance = getDistanceToCenter(targetScreenPos);

                if (distance < options::radiusFOV && distance < closestToCrosshair)
                {
                    targetScreenPosFin = targetScreenPos;
                    closestToCrosshair = distance;
                }

            }
        }

        if (isAimbotEnabled) {
            if (menu::bAimbotEnabled && targetScreenPosFin.x != 0.0f && targetScreenPosFin.y != 0.0f) {
                startAimbot(targetScreenPosFin);
            }
        }

        NO_COORDS:

        if (menu::bFOV) {
            drawFovCircle(options::radiusFOV);
        }

        if (menu::bMenuVisible)
        {
            guiStart();
            SetWindowLong(overlay.hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
        }
        else
        {
            SetWindowLong(overlay.hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        }

        overlay.RenderGui();
    }

    overlay.ReleaseOverlay();

    closeMappingHandle();
    return 0;
}