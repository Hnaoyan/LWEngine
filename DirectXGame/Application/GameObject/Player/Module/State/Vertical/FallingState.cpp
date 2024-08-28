#include "FallingState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void FallingState::Initialize()
{
	nowState_ = this;
}

void FallingState::Update()
{
	// ステート変更
	if (player_->isGround_) {
		stateManager_->ChangeRequest(StateManager::kIdle);
		return;
	}
	float gravity = -4.5f;
	player_->velocity_.y += gravity * GameSystem::GameSpeedFactor();
}

void FallingState::Exit()
{
	player_->velocity_.y = 0;
}

void FallingState::InputHandle()
{
}
