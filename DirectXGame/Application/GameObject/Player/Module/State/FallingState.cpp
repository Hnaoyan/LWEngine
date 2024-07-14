#include "FallingState.h"
#include "../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void FallingState::Initialize()
{
}

void FallingState::Update()
{
	// ステート変更
	if (player_->isGround_) {
		stateManager_->ChangeRequest(StateManager::kIdle);
		return;
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
