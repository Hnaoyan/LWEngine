#include "BoostState.h"
#include "../StateMachine.h"

void BoostState::Initialize()
{

}

void BoostState::Update()
{
	if (!isLeftStickActive_) {
		stateMachine_->ChangeRequest(HorizontalStates::kIdle);
		return;
	}
	if (!isBoost_) {
		stateMachine_->ChangeRequest(HorizontalStates::kMove);
		return;
	}
}

void BoostState::Exit()
{

}

void BoostState::InputHandle()
{
	IPlayerState::InputHandle();



}
