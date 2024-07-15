#include "IPlayerState.h"
#include "../../../Player.h"

void IPlayerState::PreInitialize(Player* player, StateMachine* stateMachine)
{
	// ポインタ
	player_ = player;
	stateMachine_ = stateMachine;

	stateManager_ = player->GetStateManager();
	input_ = Input::GetInstance();
}

void IPlayerState::InputHandle()
{
	Vector2 sThumbL = input_->XGetLeftJoystick();

	leftStick_ = sThumbL;
}
