#include "PlayerController.h"
#include "Engine/Input/Input.h"

void PlayerController::Initialize(Player* player)
{
	player_ = player;

	speedRatio_ = 5.0f;
}

void PlayerController::Update()
{
	// 移動関係
	MovementHandler();

}

void PlayerController::MovementHandler()
{
	Input* input = Input::GetInstance();
	Vector2 direct = input->XGetLeftJoystick();

	moveDirection_ = Vector2::Normalize(direct);
	velocity_ = moveDirection_ * speedRatio_;

}
