#include "BulletStateMachine.h"

void BulletStateMachine::ChangeRequest(std::unique_ptr<ITrackingState> state)
{
	// 終了時の関数
	if (currentState_) {
		currentState_->Exit();
	}
	state->SetBullet(bullet_);
	state->Enter();
	currentState_ = std::move(state);
}

void BulletStateMachine::Update()
{
	currentState_->Update();
}
