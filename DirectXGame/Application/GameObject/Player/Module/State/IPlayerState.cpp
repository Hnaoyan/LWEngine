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
	XINPUT_STATE joyState;

	leftStick_ = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX,(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
}
