#pragma once
#include "../IPlayerState.h"
#include "Engine/LwLib/LwEnginePaths.h"

/// <summary>
/// 通常移動状態
/// </summary>
class MovingState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:


};