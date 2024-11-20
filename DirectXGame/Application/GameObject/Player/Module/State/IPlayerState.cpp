#include "IPlayerState.h"
#include "../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void IPlayerState::PreInitialize(Player* player, StateManager* stateManager)
{
	player_ = player;
	stateManager_ = stateManager;
	input_ = Input::GetInstance();
}

void IPlayerState::InputHandle()
{
	Vector2 sThumbL = GameSystem::sKeyConfigManager.GetKeyConfig()->leftStick;

	leftStick_ = sThumbL;
	leftStick_ = Vector2::Normalize(leftStick_);

	if (leftStick_.x != 0.0f || leftStick_.y != 0.0f) {
		isLeftStickActive_ = true;
	}
	else {
		isLeftStickActive_ = false;
	}

}
