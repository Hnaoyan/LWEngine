#include "JumpingState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/GameSystem.h"

void JumpingState::Initialize()
{
	nowState_ = this;

	float jumpPower = 75.0f;
	player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
	// 座標更新
	player_->worldTransform_.transform_.translate += player_->velocity_;

}

void JumpingState::Update()
{
	// ステート変更
	if (player_->velocity_.y < 0) {
		stateManager_->ChangeRequest(StateManager::kFall, StateManager::kVertical);
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
	if (GameSystem::sPlayerKey.keyConfigs_.pressJump && player_->GetEnergyManager()->GetEnergyRatio() >= 0.5f)
	{
		player_->GetStateManager()->ChangeRequest(StateManager::kAssending, StateManager::kVertical);
	}
}
