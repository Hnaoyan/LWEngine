#pragma once
#include "IPlayerState.h"

class MovingState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:

};