#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../../../../Engine/Input/Input.h"
#include "../Player.h"
#include "../../../../Engine/LwLib/LwEngineLib.h"

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

	// 座標更新
	player_->worldPosition_ += player_->velocity_;
}

void OparationManager::InputUpdate()
{
	XINPUT_STATE joyState;
	float speed = 2.5f;
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
		//if (input_->TriggerKey(DIK_LSHIFT) && direct.y == 0.0f) {
		//	float highSpeed = 200.0f;
		//	player_->velocity_.z += highSpeed * GameSystem::GameSpeedFactor();
		//}
	}

	direct = Vector3::Normalize(direct);

	// 入力しているかどうか
	float slowFactor = 0.2f;
	if (direct.x == 0)
	{
		player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	}
	else {
		if (input_->TriggerKey(DIK_LSHIFT)) {
			player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * 100.0f);
		}
		player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
	}

	if (direct.y == 0) {
		player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);
	}
	else {
		if (input_->TriggerKey(DIK_LSHIFT)) {
			player_->velocity_.z += (direct.y * GameSystem::GameSpeedFactor() * 100.0f);
		}
		player_->velocity_.z += (direct.y * GameSystem::GameSpeedFactor() * speed);
	}
	// 入力による移動の速度制限
	// 左右
	if (direct.x == 0 || direct.y == 0 || speed == 20.0f) {
		return;
	}
	float maxSpeed = 1.5f;
	if (player_->velocity_.x < 0) {
		player_->velocity_.x = std::clamp(player_->velocity_.x, -maxSpeed, 0.0f);
	}
	else if (player_->velocity_.x > 0) {
		player_->velocity_.x = std::clamp(player_->velocity_.x, 0.0f, maxSpeed);
	}
	// 前後
	if (player_->velocity_.z < 0) {
		player_->velocity_.z = std::clamp(player_->velocity_.z, -maxSpeed, 0.0f);
	}
	else if (player_->velocity_.z > 0) {
		player_->velocity_.z = std::clamp(player_->velocity_.z, 0.0f, maxSpeed);
	}

}

void OparationManager::GravityUpdate()
{
	// 落下処理
	if (player_->velocity_.y != 0.0f) {
		player_->velocity_.y += (-2.5f) * GameSystem::GameSpeedFactor();
	}
	//player_->worldPosition_.y += player_->velocity_.y;
	// 仮の着地
	if (player_->worldPosition_.y <= -0.25f) {
		player_->worldPosition_.y = 0.0f;
		player_->velocity_.y = 0;
	}

}
