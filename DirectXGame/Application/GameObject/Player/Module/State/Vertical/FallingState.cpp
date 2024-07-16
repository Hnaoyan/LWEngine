#include "FallingState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void FallingState::Initialize()
{
	IPlayerState::Initialize();

}

void FallingState::Update()
{
	// ステート変更
	if (player_->isGround_) {
		if (stateMachine_) {
			stateMachine_->ChangeRequest(VerticalStates::kIdle);
			return;
		}
		else {
			//stateManager_->ChangeRequest(StateManager::kIdle);
			return;
		}
	}
	
	player_->velocity_.y += (-4.5f) * GameSystem::GameSpeedFactor();
}

void FallingState::Exit()
{
	player_->velocity_.y = 0;
}

void FallingState::InputHandle()
{
}
