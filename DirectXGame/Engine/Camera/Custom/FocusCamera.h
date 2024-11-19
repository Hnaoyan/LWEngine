#pragma once
#include "../Core/ICamera.h"
#include "Engine/3D/WorldTransform.h"

/// <summary>
/// 注視カメラ
/// </summary>
class FocusCamera : public ICamera
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

public:
	/// <summary>
	/// 角度の更新
	/// </summary>
	void FocusUpdate();

public:
	void SetFocusPoint(Vector3* focusPoint) { focusPoint_ = focusPoint; }
	void SetWorldTransform(WorldTransform* worldTransform) { worldTransform_ = worldTransform; }
private:
	bool isQuaternion_ = true;
	// 注視座標
	WorldTransform* worldTransform_ = nullptr;
	Vector3* focusPoint_ = nullptr;
	// 回転用のクォータニオン
	Quaternion rotateQuaternion_{};

};
