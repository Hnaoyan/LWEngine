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
	// ロックオン
	lockOn_.Initialize(player);
	// Aim
	aimManager_.Initialize(player);
}

void OparationManager::Update()
{
	// 落下処理
	GravityUpdate();
	// 入力
	InputUpdate();
	// ターゲットが死んだ場合解除するための更新
	lockOn_.Update();
	// Aimの処理
	aimManager_.Update(player_->camera_);
	// クールタイム
	shotTimer_.Update(GameSystem::sSpeedFactor);

}

void OparationManager::InputUpdate()
{
	float speed = 4.0f;
	Vector3 direct = {};
	Vector2 sThumbL = input_->XGetLeftJoystick();
	// コントローラー操作
	// 方向取得
	direct = { sThumbL.x,sThumbL.y ,0 };
	// ジャンプ入力
	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_A) && player_->velocity_.y == 0.0f)
	{
		player_->GetVerticalState()->ChangeRequest(VerticalStates::kJump);
		//player_->GetHorizontalState()->ChangeRequest(VerticalStates::kJump);
		//player_->GetStateManager()->ChangeRequest(StateManager::kJump);
		//float jumpPower = 75.0f;
		//player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
	}
		
	if (input_->XRTrigger() && !shotTimer_.isActive_) {
		SampleBulletManager::GenerateData data{};
		data.position = player_->worldTransform_.GetWorldPosition();
		data.velocity = Vector3::Normalize(aimManager_.GetWorldPosition() - player_->worldTransform_.GetWorldPosition());
		float speedValue = 30.0f;
		data.velocity *= speedValue;
		bulletManager_->AddBullet(data);
		shotTimer_.Start(30.0f);
	}

	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B)) {
		lockOn_.ToggleLockOn(player_->camera_);
	}

	direct = Vector3::Normalize(direct);

	// カメラの回転に方向を適応
	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	player_->worldTransform_.transform_.rotate.y = player_->camera_->transform_.rotate.y;
	direct = Matrix4x4::TransformVector3({ direct.x,0,direct.y }, rotateY);

	// 入力しているかどうか
	float slowFactor = 0.2f;
	// 速度処理
	player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
	player_->velocity_.z += (direct.z * GameSystem::GameSpeedFactor() * speed);
	// 減速処理
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

	// 入力による移動の速度制限
	// 左右
	//if (direct.x == 0 || direct.z == 0 || speed == 20.0f) {
	//	return;
	//}
	if (isDash_) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
		++resetTime_;
		if (resetTime_ > 10) {
			isDash_ = false;
			resetTime_ = 0;
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;

		}
		return;
	}

	if (direct.x != 0 || direct.z != 0) {
		if (!isDash_ && input_->XTriggerJoystick(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			isDash_ = true;
			float dashPower = 100.0f * GameSystem::GameSpeedFactor();
			player_->velocity_.x = direct.x * dashPower;
			player_->velocity_.z = direct.z * dashPower;
		}
	}
	
}

void OparationManager::GravityUpdate()
{
	//if (!player_->isGround_) {
	//	player_->velocity_.y += (-4.5f) * GameSystem::GameSpeedFactor();
	//}
	//else {
	//	player_->velocity_.y = 0;
	//}
	
}
