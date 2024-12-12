#pragma once
#include "../IPlayerState.h"
#include "Engine/LwLib/LwEnginePaths.h"

/// <summary>
/// ジャンプの上り状態
/// </summary>
class JumpingState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
	// 2段ジャンプまでのクールタイム
	FrameTimer jumpCooltime_;
	bool isSecondJump_ = false;
	// ジャンプの受付時の処理
	void JumpActionExecute();
};