#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../Player.h"
#include "../../Bullet/SampleBulletManager.h"

#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/PostEffect/PostEffectRender.h"

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

	shotTimer_.Update(GameSystem::sSpeedFactor);

	// 座標更新
	player_->worldTransform_.transform_.translate += player_->velocity_;
}

void OparationManager::InputUpdate()
{
	XINPUT_STATE joyState;
	float speed = 1.5f;
	Vector3 direct = {};
	// コントローラー操作
	if (input_->GetJoystickState(0, joyState)) {
		// 方向取得
		direct = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX,(float)joyState.Gamepad.sThumbLY / SHRT_MAX ,0 };
		// ジャンプ入力
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && player_->velocity_.y == 0.0f)
		{
			float jumpPower = 50.0f;
			player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
		}
		
		if (joyState.Gamepad.bRightTrigger && !shotTimer_.isActive_) {
			SampleBulletManager::GenerateData data{};
			data.position = player_->worldTransform_.GetWorldPosition();
			data.velocity = Vector3::Normalize(player_->GetAimReticle() - player_->worldTransform_.GetWorldPosition());
			float speedValue = 30.0f;
			data.velocity *= speedValue;
			bulletManager_->AddBullet(data);
			shotTimer_.Start(30.0f);
		}

		if (input_->PressKey(DIK_G)) {
			GameSystem::sSpeedFactor = 5.0f;
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kGrayScale;
		}
		else {
			GameSystem::sSpeedFactor = 1.0f;
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
		}

		//if (input_->GetJoystickState(0, joyState)) {
		//	player_->worldTransform_.transform_.rotate.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 0.01f;
		//}

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

	direct = Vector3::Normalize(direct);

	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);

	player_->worldTransform_.transform_.rotate.y = player_->camera_->transform_.rotate.y;

	Vector3 rotateVector = Matrix4x4::TransformVector3({ direct.x,0,direct.y }, rotateY);
	direct = rotateVector;

	// 入力しているかどうか
	float slowFactor = 0.2f;
	if (direct.x == 0)
	{
		//player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	}
	else {
		if (input_->TriggerKey(DIK_LSHIFT)) {
			player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * 100.0f);
		}
		player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
	}
	if (direct.z == 0) {
		//player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);
	}
	else {
		if (input_->TriggerKey(DIK_LSHIFT)) {
			player_->velocity_.z += (direct.z * GameSystem::GameSpeedFactor() * 100.0f);
		}
		player_->velocity_.z += (direct.z * GameSystem::GameSpeedFactor() * speed);
	}
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

	// 入力による移動の速度制限
	// 左右
	if (direct.x == 0 || direct.z == 0 || speed == 20.0f) {
		return;
	}
	if (isDash_) {
		++resetTime_;
		if (resetTime_ > 10) {
			isDash_ = false;
			resetTime_ = 0;
		}
		return;
	}

	//float maxSpeed = 1.5f;
	//if (player_->velocity_.x < 0) {
	//	player_->velocity_.x = std::clamp(player_->velocity_.x, -maxSpeed, 0.0f);
	//}
	//else if (player_->velocity_.x > 0) {
	//	player_->velocity_.x = std::clamp(player_->velocity_.x, 0.0f, maxSpeed);
	//}
	//// 前後
	//if (player_->velocity_.z < 0) {
	//	player_->velocity_.z = std::clamp(player_->velocity_.z, -maxSpeed, 0.0f);
	//}
	//else if (player_->velocity_.z > 0) {
	//	player_->velocity_.z = std::clamp(player_->velocity_.z, 0.0f, maxSpeed);
	//}

	if (direct.x != 0 || direct.z != 0) {
		if (!isDash_ && joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			isDash_ = true;
			float dashPower = 100.0f * GameSystem::GameSpeedFactor();
			player_->velocity_.x = direct.x * dashPower;
			player_->velocity_.z = direct.z * dashPower;
		}
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
	if (player_->worldTransform_.transform_.translate.y <= -0.25f) {
		player_->worldTransform_.transform_.translate.y = 0.0f;
		player_->velocity_.y = 0;
	}

}