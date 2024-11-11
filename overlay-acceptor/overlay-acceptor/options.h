#pragma once

namespace menu {
	inline bool bMenuVisible{ true };

	inline bool bEspEnabled{ false };
	inline bool bPointsEnabled{ false };
	inline bool bLinesEnabled{ false };
	inline bool bAimbotEnabled{ false };
	inline bool bFOV{ false };

	inline unsigned short bToggleESP{ 0x0 };
	inline unsigned short bToggleButton{ 0x2d };
	inline unsigned short bEndButton{ 0x23 };
}

namespace options {
	inline ImVec4 linesColor{ 1.0f, 0.0f, 0.0f, 1.0f };
	inline ImVec4 pointsColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	inline int selectedAimbot{ 0 };
	inline float monitorX;
	inline float monitorY;
	inline float radiusFOV{70.0f};
	inline float smoothness{2.0f};
	inline float circleRadius{2.0f};
}