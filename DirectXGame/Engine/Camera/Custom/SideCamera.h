#pragma once
#include "../Core/ICamera.h"
#include "../../3D/WorldTransform.h"

class SideCamera : public ICamera
{
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
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue() override;
public:
	void SetParent(const WorldTransform* transform) { worldTransform_.parent_ = transform; }

private:
	// 親子関係を組む用
	WorldTransform worldTransform_;

};
