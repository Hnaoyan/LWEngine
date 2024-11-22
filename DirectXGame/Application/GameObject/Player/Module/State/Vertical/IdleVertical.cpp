#include "IdleVertical.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"

void IdleVertical::Initialize()
{
	nowState_ = this;
}

void IdleVertical::Update()
{
	// ステート変更
	if (!player_->isGround_) {
		stateMachine_->ChangeRequest(PlayerStateLists::kFall);
		return;
	}

}

void IdleVertical::Exit()
{
}

void IdleVertical::InputHandle()
{
	// ジャンプキー
	if (GameSystem::sKeyConfigManager.GetPlayerKey().jump)
	{
		stateMachine_->ChangeRequest(PlayerStateLists::kJump);
	}
}
