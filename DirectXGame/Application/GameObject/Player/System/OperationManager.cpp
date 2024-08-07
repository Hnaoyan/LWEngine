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
	// 入力
	InputUpdate();
	// ターゲットが死んだ場合解除するための更新
	lockOn_.Update();
	// Aimの処理
	aimManager_.Update(player_->camera_);
	// クールタイム
	dashCooltime_.Update(GameSystem::GameSpeedFactor());
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
	Vector2 sThumbR = input_->XGetRightJoystick();
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

		if (lockOn_.ExistTarget()) {
			speedValue *= 2.0f;
		}

		velocity *= speedValue;
		EulerTransform transform{};
		transform.scale = { 1.0f,1.0f,1.0f };
		transform.translate = player_->worldTransform_.GetWorldPosition();
		bulletManager_->GetBeginCluster()->AddBullet(transform, velocity);
		shotTimer_.Start(30.0f);
	}

	// カメラの処理
	if (input_->XTriggerJoystick(XINPUT_GAMEPAD_B) && !lockOnCooltime_.IsActive()) {
		lockOn_.ToggleLockOn(player_->camera_);
		lockOnCooltime_.Start(20.0f);
	}
	// スティックでロックオン対象を変更
	if (lockOn_.ExistTarget() && (sThumbR.x != 0 || sThumbR.y != 0) && !lockOnCooltime_.IsActive()) {
		lockOn_.ChangeLockOnTarget(player_->camera_);
		lockOnCooltime_.Start(20.0f);
	}

	direct = Vector3::Normalize(direct);

	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ direct.x,0,direct.y }, rotateY);
	direct = rotateVector;

	//player_->worldTransform_.transform_.rotate.y = player_->camera_->transform_.rotate.y;
	Vector3 sub = Vector3::Normalize({ sThumbL.x,sThumbL.y ,0 });
	if (sub.x != 0.0f || sub.y != 0.0f) {
		sub = Matrix4x4::TransformVector3({ sub.x,0,sub.y }, rotateY);
		player_->worldTransform_.transform_.rotate.y = LwLib::CalculateYawFromVector({ sub.x,0,sub.z });
	}

	float slowFactor = 0.2f;

	// ダッシュ中の処理
	if (isDash_) {
		PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
		if (resetTime_ > 10) {
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;

		}
		++resetTime_;


		dashVelocity_.x = LwLib::Lerp(dashVelocity_.x, 0, resetTime_ / 80.0f);
		dashVelocity_.z = LwLib::Lerp(dashVelocity_.z, 0, resetTime_ / 80.0f);

		player_->velocity_.x += dashVelocity_.x * GameSystem::GameSpeedFactor();
		player_->velocity_.z += dashVelocity_.z * GameSystem::GameSpeedFactor();

		if (dashVelocity_.x == 0.0f && dashVelocity_.z == 0.0f) {
			isDash_ = false;
			resetTime_ = 0;
			dashCooltime_.Start(300.0f);
			return;
		}

	}
	else {
		// 入力しているかどうか
		if (direct.x != 0)
		{
			player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
		}
		if (direct.z != 0) {
			player_->velocity_.z += (direct.z * GameSystem::GameSpeedFactor() * speed);
		}

		// ダッシュの入力
		if (direct.x != 0 || direct.z != 0) {
			if (!isDash_ && input_->XTriggerJoystick(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				if (dashCooltime_.IsActive()) {
					return;
				}
				isDash_ = true;
				float dashPower = 40.0f;
				dashVelocity_.x = direct.x * dashPower;
				dashVelocity_.z = direct.z * dashPower;

				//player_->velocity_.x = direct.x * dashPower;
				//player_->velocity_.z = direct.z * dashPower;
			}
		}
	}

	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

}
