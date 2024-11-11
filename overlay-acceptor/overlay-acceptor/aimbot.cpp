#include <Windows.h> 
#include <cmath>
#include "aimbot.h"
#include "imgui/imgui.h"
#include "options.h"
#include <chrono>

#define M_PI 3.14159265358979323846

void moveMouseByOffset(int offsetX, int offsetY) {
    POINT pt;
    if (GetCursorPos(&pt)) {
        int newX = pt.x + offsetX;
        int newY = pt.y + offsetY;

        mouse_event(MOUSEEVENTF_MOVE, offsetX, offsetY, 0, 0);
    }
}

float getDistanceToCenter(const ImVec2& screenPos) {
    return sqrt(
        (options::monitorX / 2 - screenPos.x) * (options::monitorX / 2 - screenPos.x) +
        (options::monitorY / 2 - screenPos.y) * (options::monitorY / 2 - screenPos.y)
    );
}

void startAimbot(ImVec2& coords) {
    ImVec2 screenCenter{ options::monitorX / 2.f, options::monitorY / 2.f };

    moveMouseByOffset(-(screenCenter.x - coords.x) / options::smoothness, -(screenCenter.y - coords.y) / options::smoothness);
}