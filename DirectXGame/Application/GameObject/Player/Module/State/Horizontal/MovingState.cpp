#include "MovingState.h"
#include "../StateMachine.h"

void MovingState::Initialize()
{
}

void MovingState::Update()
{
	if (!isLeftStickActive_) {
		stateMachine_->ChangeRequest(HorizontalStates::kIdle);
		return;
	}

	if (isBoost_) {
		stateMachine_->ChangeRequest(HorizontalStates::kBoost);
		return;
	}
}

void MovingState::Exit()
{
}

void MovingState::InputHandle()
{
	IPlayerState::InputHandle();



}
