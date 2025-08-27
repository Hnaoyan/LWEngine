#pragma once
#include "Engine/Camera/Core/ICamera.h"
#include "Engine/3D/WorldTransform.h"

class FollowCamera2D : public ICamera
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

private:
	/// <summary>
	/// 追従更新
	/// </summary>
	void FollowUpdate();

public:
	void SetTarget(WorldTransform* target) { target_ = target; }

private:
	// 追従対象
	WorldTransform* target_ = nullptr;
	float delayRate_ = 0.0f;
	Vector3 interTarget_ = {};
	float offsetZ = -50.0f;
};

