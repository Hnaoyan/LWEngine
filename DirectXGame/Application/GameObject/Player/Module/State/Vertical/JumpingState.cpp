#include "JumpingState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/GameSystem.h"

void JumpingState::Initialize()
{
	nowState_ = this;

	float jumpPower = 65.0f;
	player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
	// 座標更新
	player_->worldTransform_.transform_.translate += player_->velocity_;

}

void JumpingState::Update()
{
	// ステート変更
	if (player_->velocity_.y < 0) {
		stateManager_->ChangeRequest(StateManager::kFall);
		return;
	}
	float gravity = -4.5f;
	player_->velocity_.y += gravity * GameSystem::GameSpeedFactor();
}

void JumpingState::Exit()
{
}

void JumpingState::InputHandle()
{
	float energyRatio = player_->GetSystemFacede()->GetEnergy()->GetEnergyRatio();
	if (GameSystem::sPlayerKey.keyConfigs_.pressJump && energyRatio >= 0.5f)
	{
		player_->VerticalManager()->ChangeRequest(StateManager::kAssending);
	}
}
