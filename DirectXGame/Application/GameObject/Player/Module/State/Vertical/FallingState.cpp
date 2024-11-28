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
		stateMachine_->ChangeRequest(PlayerStateLists::kIdleVertical);
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
	float energyRatio = player_->GetSystemFacede()->GetEnergy()->GetEnergyRatio();
	if (GameSystem::sKeyConfigManager.GetPlayerKey().pressJump && energyRatio >= 0.5f)
	{
		player_->VerticalState()->ChangeRequest(PlayerStateLists::kAssending);
	}

	if (GameSystem::sKeyConfigManager.GetPlayerKey().jump && !player_->isDoubleJump_) {
		player_->VerticalState()->ChangeRequest(PlayerStateLists::kJump);
		player_->isDoubleJump_ = true;
	}
}
