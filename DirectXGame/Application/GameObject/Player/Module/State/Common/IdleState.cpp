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
		//stateMachine_->ChangeRequest(VerticalStates::kFall);
		//stateManager_->ChangeRequest(StateManager::kFall);
		player_->GetVerticalState()->ChangeRequest(VerticalStates::kFall);
		return;
	}

}

void IdleState::Exit()
{

}

void IdleState::InputHandle()
{
	if (input_->TriggerKey(DIK_L)) {
		//stateMachine_->ChangeRequest(VerticalStates::kJump);
		stateManager_->ChangeRequest(StateManager::kJump);
		return;
	}
}
