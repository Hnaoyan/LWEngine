#include "PlayerController.h"
#include "../Player.h"

#include "Engine/Input/Input.h"
#include "Engine/LwLib/DeltaTime.h"

void PlayerController::Initialize(Player* player)
{
	player_ = player;

	speedRatio_ = 5.0f;
}

void PlayerController::Update()
{
	// 速度リセット
	velocity_ = {};
	// 移動関係
	MovementHandler();

	// 座標移動
	Vector2 worldPosition = Vector2(player_->GetWorldTransform()->GetWorldPosition().x, player_->GetWorldTransform()->GetWorldPosition().y);
	worldPosition += velocity_ * kDeltaTime;
	player_->GetWorldTransform()->transform_.translate = Vector3(worldPosition.x, worldPosition.y, 0.0f);
}

void PlayerController::MovementHandler()
{
	Input* input = Input::GetInstance();
	Vector2 direct = input->XGetLeftJoystick();
	if (input->PressKey(DIK_A)) {
		direct.x = -1.0f;
	}
	else if (input->PressKey(DIK_D)) {
		direct.x = 1.0f;
	}
	else {
		direct.x = 0.0f;
	}

	moveDirection_ = Vector2::Normalize(direct);
	velocity_ = moveDirection_ * speedRatio_;

}
