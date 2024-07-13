#include "IPlayerState.h"
#include "../../Player.h"

void IPlayerState::PreInitialize(Player* player)
{
	player_ = player;
	stateManager_ = player->GetStateManager();
	input_ = Input::GetInstance();
}

void IPlayerState::InputHandle()
{
	Vector2 sThumbL = input_->XGetLeftJoystick();

	leftStick_ = sThumbL;
}
