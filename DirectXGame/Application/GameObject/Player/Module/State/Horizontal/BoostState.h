#pragma once
#include "../IPlayerState.h"
#include "Engine/LwLib/LwEnginePaths.h"

/// <summary>
/// ダッシュ状態
/// </summary>
class BoostState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
	FrameTimer changeTimer_;	// 変更までの時間
	Vector3 dashVelocity_{};	// 速度

};
