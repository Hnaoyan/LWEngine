#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../../../../Engine/Input/Input.h"
#include "../Player.h"

void OparationManager::Initialize(Player* player)
{
	assert(player);
	// プレイヤー
	player_ = player;
	// 入力
	input_ = Input::GetInstance();
}

void OparationManager::Update()
{
	// 入力
	InputUpdate();

	// 落下処理
	GravityUpdate();
}

void OparationManager::InputUpdate()
{
	XINPUT_STATE joyState;
	float speed = 4.0f;
	Vector3 direct = {};
	// コントローラー操作
	if (input_->GetJoystickState(0, joyState)) {
		direct = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX,(float)joyState.Gamepad.sThumbLY / SHRT_MAX ,0 };
		if ((direct.x != 0 || direct.y != 0) && joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			player_->worldPosition_.x += (direct.x * GameSystem::GameSpeedFactor() * 20.0f);
			player_->worldPosition_.z += (direct.y * GameSystem::GameSpeedFactor() * 20.0f);
		}
	}
	// キーボード操作
	else {
		if (input_->PressKey(DIK_A))
		{
			direct.x -= 1.0f;
		}
		else if (input_->PressKey(DIK_D)) 
		{
			direct.x += 1.0f;
		}

		if (input_->PressKey(DIK_W)) {
			direct.y += 1.0f;
		}
		else if (input_->PressKey(DIK_S)) {
			direct.y -= 1.0f;
		}

		if (input_->TriggerKey(DIK_SPACE) && player_->velocity_.y == 0.0f) {
			float jumpPower = 50.0f;
			player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
		}
	}

	// 座標移動（ここ後で変更する
	if (direct.x == 0)
	{
		//player_->velocity_.x = 
	}
	player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
	player_->velocity_.z += (direct.y * GameSystem::GameSpeedFactor() * speed);
	if (player_->velocity_.x < 0) {
		player_->velocity_.x = std::clamp(player_->velocity_.x, -1.0f, 0.0f);
	}
	else if (player_->velocity_.x > 0) {
		player_->velocity_.x = std::clamp(player_->velocity_.x, 0.0f, 1.0f);
	}

}

void OparationManager::GravityUpdate()
{
	// 落下処理
	if (player_->velocity_.y != 0.0f) {
		player_->velocity_.y += (-2.5f) * GameSystem::GameSpeedFactor();
	}
	// 座標更新
	player_->worldPosition_ += player_->velocity_;
	//player_->worldPosition_.y += player_->velocity_.y;
	// 仮の着地
	if (player_->worldPosition_.y <= -0.25f) {
		player_->worldPosition_.y = 0.0f;
		player_->velocity_.y = 0;
	}

}
