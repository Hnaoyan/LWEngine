#pragma once
#include "IPlayerState.h"

class JumpingState : public IPlayerState
{
public:
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:
};