#pragma once
#include "../ITrackingState.h"

class TurnToTargetState : public ITrackingState
{
public:
	void Enter() override;
	void Update(BulletStateMachine& stateMachine) override;
	void Exit() override;

private:
	// 旋回タイミングのタイマー
	FrameTimer turnTimer_;
	// 移動方向
	Vector3 moveDirection_ = {};
};
