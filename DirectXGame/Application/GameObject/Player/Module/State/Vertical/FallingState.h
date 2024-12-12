#pragma once
#include "../IPlayerState.h"

/// <summary>
/// 落下状態
/// </summary>
class FallingState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:

};