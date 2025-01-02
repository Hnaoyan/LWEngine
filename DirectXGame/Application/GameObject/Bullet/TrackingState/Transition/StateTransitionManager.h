#pragma once
#include "Engine/LwLib/LwEnginePaths.h"

class BulletStateMachine;

class StateTransitionManager
{
public:
	void Initialize(BulletStateMachine* stateMachine);
	void Update();

private:
	// 親のステート
	BulletStateMachine* stateMachine_ = nullptr;
	// 遷移までの時間
	FrameTimer transitionTimer_;
};
