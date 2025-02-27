#pragma once
#include "../IPlayerState.h"
#include "Engine/LwLib/LwEnginePaths.h"

/// <summary>
/// 小ダッシュ状態
/// </summary>
class QuickBoostState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
	FrameTimer changeTimer_;
	Vector3 dashVelocity_{};

};
