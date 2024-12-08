#pragma once
#include "../IPlayerState.h"

/// <summary>
/// 滞空ステート
/// </summary>
class HoverState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

};

