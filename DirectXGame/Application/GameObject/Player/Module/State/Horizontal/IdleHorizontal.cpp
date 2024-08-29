#include "IdleHorizontal.h"
#include "Application/GameObject/Player/Player.h"

void IdleHorizontal::Initialize()
{
	nowState_ = this;
}

void IdleHorizontal::Update()
{
	//player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	//player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

	if (isLeftStickActive_) {
		stateManager_->ChangeRequest(StateManager::kMove, StateManager::kHorizontal);
		return;
	}
}

void IdleHorizontal::Exit()
{

}

void IdleHorizontal::InputHandle()
{
	// 入力
	IPlayerState::InputHandle();

}
