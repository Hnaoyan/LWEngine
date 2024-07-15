#include "IdleState.h"
#include "../../../Player.h"
#include "../StateMachine.h"

void IdleState::Initialize()
{

}

void IdleState::Update()
{
	// ステート変更
	if (!player_->isGround_) {
		if (stateMachine_) {
			if (stateMachine_->name_ != "Vertical") {
				return;
			}
			stateMachine_->ChangeRequest(VerticalStates::kFall);
		}
		else {
			stateManager_->ChangeRequest(StateManager::kFall);
		}
		return;
	}

}

void IdleState::Exit()
{

}

void IdleState::InputHandle()
{
	if (input_->TriggerKey(DIK_L)) {
		if (stateMachine_) {
			stateMachine_->ChangeRequest(VerticalStates::kJump);
		}
		else {
			stateManager_->ChangeRequest(StateManager::kJump);
		}
		return;
	}
}
