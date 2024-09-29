#include "IPlayerState.h"
#include "../../Player.h"

void IPlayerState::PreInitialize(Player* player, StateManager* stateManager)
{
	player_ = player;
	stateManager_ = stateManager;
	input_ = Input::GetInstance();
}

void IPlayerState::InputHandle()
{
	Vector2 sThumbL = input_->XGetLeftJoystick();

	leftStick_ = sThumbL;
	leftStick_ = Vector2::Normalize(leftStick_);

	if (leftStick_.x != 0.0f || leftStick_.y != 0.0f) {
		isLeftStickActive_ = true;
	}
	else {
		isLeftStickActive_ = false;
	}

}
