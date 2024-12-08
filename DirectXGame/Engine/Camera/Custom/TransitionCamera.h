#pragma once
#include "../Core/ICamera.h"
#include "Engine/3D/WorldTransform.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <utility>

class TransitionCamera : public ICamera
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


	void BeginTransition(const Vector3& startPoint, const Vector3& endPoint, const float& frame);
	bool TransitionEnd() { return transitionTimer_.IsEnd(); }
private:
	// 開始地点、終了地点
	std::pair<Vector3, Vector3> easePoints_;
	// 注視する地点
	Vector3 focusPoint_{};
	// 遷移時間
	FrameTimer transitionTimer_;

};
