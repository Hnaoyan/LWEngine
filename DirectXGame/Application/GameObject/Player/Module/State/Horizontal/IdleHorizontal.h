#pragma once
#include "../IPlayerState.h"

/// <summary>
/// 待機状態（水平：XZ平面用
/// </summary>
class IdleHorizontal : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
};