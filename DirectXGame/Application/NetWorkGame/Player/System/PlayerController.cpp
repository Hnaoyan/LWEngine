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
	player_->GetWorldTransform()->UpdateMatrix();
	// 速度リセット
	velocity_.x = 0;
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
	//if (input->PressKey(DIK_W)) {
	//	direct.y = 1.0f;
	//}
	//else if (input->PressKey(DIK_S)) {
	//	direct.y = -1.0f;
	//}
	//else {
	//	direct.y = 0.0f;
	//}
	direct.y = 0.0f;
	moveDirection_ = Vector2::Normalize(direct);
	velocity_.x = moveDirection_.x * speedRatio_;

	if (input->TriggerKey(DIK_SPACE)) {
		velocity_.y += 50.0f;
	}

	// 減速
	velocity_.y = velocity_.y * 0.85f;
	if (velocity_.y <= 0.01f) {
		velocity_.y = 0.0f;
	}

}
