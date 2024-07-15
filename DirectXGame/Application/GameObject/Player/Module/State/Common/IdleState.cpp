#include "IdleState.h"
#include "../../../Player.h"
#include "../StateMachine.h"

void IdleState::Initialize()
{

}

void IdleState::Update()
{
	switch (stateMachine_->GetStateType())
	{
	case StateMachine::StateType::kVertical:
		// ステート変更
		if (!player_->isGround_) {
			if (stateMachine_) {
				stateMachine_->ChangeRequest(VerticalStates::kFall);
				return;
			}
			else {
				stateManager_->ChangeRequest(StateManager::kFall);
				return;
			}
		}
		break;
	case StateMachine::StateType::kHorizontal:
		if (leftStick_.x != 0.0f || leftStick_.y != 0.0f) {
			if (isBoost_) {
				// ブーストに
				stateMachine_->ChangeRequest(HorizontalStates::kBoost);
				return;
			}
			else {
				// 通常移動に
				stateMachine_->ChangeRequest(HorizontalStates::kMove);
				return;
			}
		}

		break;
	default:
		break;
	}

}

void IdleState::Exit()
{

}

void IdleState::InputHandle()
{
	// 入力
	IPlayerState::InputHandle();

}
