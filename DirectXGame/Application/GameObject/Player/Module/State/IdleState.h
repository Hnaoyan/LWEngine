#pragma once
#include "IPlayerState.h"

class IdleState : public IPlayerState
{
public:
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:

};
