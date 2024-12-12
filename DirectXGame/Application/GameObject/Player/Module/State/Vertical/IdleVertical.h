#pragma once
#include "../IPlayerState.h"

/// <summary>
/// 待機状態（Y軸
/// </summary>
class IdleVertical : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
};