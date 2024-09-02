#include "AssendingState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void AssendingState::Initialize()
{
	nowState_ = this;
}

void AssendingState::Update()
{
	float energy = player_->GetSystemFacede()->GetEnergy()->GetNowEnergy();
	if (energy <= 0.0f) {
		stateManager_->ChangeRequest(StateManager::kFall, StateManager::kVertical);
		return;
	}
	float assendPower = 3.0f;
	float gravity = -2.5f;
	player_->velocity_.y += assendPower * GameSystem::GameSpeedFactor();
	player_->velocity_.y += gravity * GameSystem::GameSpeedFactor();
}

void AssendingState::Exit()
{

}

void AssendingState::InputHandle()
{
	if (!(GameSystem::sPlayerKey.keyConfigs_.pressJump)) {
		stateManager_->ChangeRequest(StateManager::kFall, StateManager::kVertical);
		return;
	}
}
