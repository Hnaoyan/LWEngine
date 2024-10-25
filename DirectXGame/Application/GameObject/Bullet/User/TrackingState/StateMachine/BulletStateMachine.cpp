#include "BulletStateMachine.h"
#include "../TrackingStates.h"

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

void BulletStateMachine::ChangeRequest(TrackingState state)
{
	if (currentState_) {
		currentState_->Exit();
	}
	std::unique_ptr<ITrackingState> newState = BuildState(state);

	newState->SetBullet(bullet_);
	newState->Enter();
	currentState_ = std::move(newState);
}

std::unique_ptr<ITrackingState> BulletStateMachine::BuildState(TrackingState newState)
{
	std::unique_ptr<ITrackingState> instance;

	switch (newState)
	{
	case TrackingState::kStraight:
		instance = std::make_unique<TrakingStraightState>();
		break;
	case TrackingState::kWave:
		instance = std::make_unique<TrackingWaveringState>();
		break;
	case TrackingState::kTracking:
		break;
	}

	return std::move(instance);
}

void BulletStateMachine::Update()
{
	currentState_->Update(*this);
}
