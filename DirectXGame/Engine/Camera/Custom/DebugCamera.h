#pragma once
#include "../Core/ICamera.h"

class DebugCamera : public ICamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

private:


};