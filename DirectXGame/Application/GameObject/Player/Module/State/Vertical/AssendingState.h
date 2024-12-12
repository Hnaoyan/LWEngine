#pragma once
#include "../IPlayerState.h"

/// <summary>
/// 上昇状態
/// </summary>
class AssendingState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
};