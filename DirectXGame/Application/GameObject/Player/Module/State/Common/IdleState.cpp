#include "IdleState.h"
#include "../../../Player.h"

#include "Engine/LwLib/LwEngineLib.h"

void IdleState::Initialize()
{
	IPlayerState::Initialize();
}

void IdleState::Update()
{
	StateMachine::StateType type = stateMachine_->GetStateType();

	(this->*updateFunc[static_cast<uint32_t>(type)].updateFunc_)();
}

void IdleState::Exit()
{

}

void IdleState::InputHandle()
{
	// 入力
	IPlayerState::InputHandle();

}

void IdleState::VerticalUpdate()
{
	// ステート変更
	if (!player_->isGround_) {
		if (stateMachine_) {
			stateMachine_->ChangeRequest(VerticalStates::kFall);
			return;
		}
		else {
			//stateManager_->ChangeRequest(StateManager::kFall);
			return;
		}
	}
}

void IdleState::HorizontalUpdate()
{
	float slowFactor = 0.2f;
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

	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);
}
