#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../Player.h"
#include "../../Bullet/BulletManager.h"

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
	lockOnCooltime_.Update(GameSystem::sSpeedFactor);
	// 座標更新
	player_->worldTransform_.transform_.translate += player_->velocity_;
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
		player_->GetStateManager()->ChangeRequest(StateManager::kJump);
	}
	// 射撃入力
	if (input_->XRTrigger() && !shotTimer_.IsActive()) {
		Vector3 velocity = Vector3::Normalize(aimManager_.GetWorldPosition() - player_->worldTransform_.GetWorldPosition());
		float speedValue = 30.0f;
		velocity *= speedValue;
		EulerTransform transform{};
		transform.scale = { 1.0f,1.0f,1.0f };
		transform.translate = player_->worldTransform_.GetWorldPosition();
		bulletManager_->GetBeginCluster()->AddBullet(transform, velocity);
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

	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B) && !lockOnCooltime_.IsActive()) {
		lockOn_.ToggleLockOn(player_->camera_);
		lockOnCooltime_.Start(20.0f);
	}

	// キーボード操作
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
