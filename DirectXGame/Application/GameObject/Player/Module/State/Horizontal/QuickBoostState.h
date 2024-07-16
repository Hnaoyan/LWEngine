#pragma once
#include "../Common/IPlayerState.h"

class QuickBoostState : public IPlayerState
{
public: // 継承部
	void Initialize() override;
	void Update() override;
	void Exit() override;
	void InputHandle() override;

private:

};
