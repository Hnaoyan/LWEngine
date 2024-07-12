#include "JumpingState.h"
#include "../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void JumpingState::Initialize()
{
	float jumpPower = 75.0f;
	player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
	// 座標更新
	player_->worldTransform_.transform_.translate += player_->velocity_;

}

void JumpingState::Update()
{

	if (player_->velocity_.y < 0) {
		stateManager_->ChangeRequest(StateManager::kFall);
	}

}

void JumpingState::Exit()
{
}

void JumpingState::InputHandle()
{
}
