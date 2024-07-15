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
	// 左スティックの値
	Vector2 sThumbL = input_->XGetLeftJoystick();
	leftStick_ = sThumbL;

	// スティックを倒しているか
	if (leftStick_.x != 0.0f || leftStick_.y != 0.0f) {
		isLeftStickActive_ = true;
	}
	else {
		isLeftStickActive_ = false;
	}
	// ブーストかどうか
	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B)) {
		if (isBoost_) {
			isBoost_ = false;
		}
		else {
			isBoost_ = true;
		}
	}
}
