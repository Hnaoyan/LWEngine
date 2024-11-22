#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../Player.h"
#include "../../Bullet/BulletManager.h"
#include "Application/GameSystem/GameSystem.h"

#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwLibrary.h"
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
	Vector3 direct = {};
	// スティックごとの入力値
	Vector2 sThumbL = GameSystem::sKeyConfigManager.GetKeyConfig()->leftStick;
	Vector2 sThumbR = GameSystem::sKeyConfigManager.GetKeyConfig()->rightStick;
	
	// 方向設定
	direct = { sThumbL.x,sThumbL.y ,0 };
	//---射撃入力---//
	// 通常
	if (GameSystem::sKeyConfigManager.GetPlayerKey().shot && !shotTimer_.IsActive()) {
		Vector3 velocity = Vector3::Normalize(aimManager_.GetWorldPosition() - player_->worldTransform_.GetWorldPosition());
		player_->GetSystemFacede()->GetShootingManager()->OnFire(velocity);
		shotTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "ShotDuration"));
	}
	// 追従
	else if (GameSystem::sKeyConfigManager.GetPlayerKey().homingShot && !shotTimer_.IsActive()) {
		player_->GetSystemFacede()->GetShootingManager()->TrackingFire();
		shotTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "ShotDuration"));
	}

	// カメラの処理
	if (GameSystem::sKeyConfigManager.GetPlayerKey().lockon && !lockOnCooltime_.IsActive()) {
		lockOn_.ToggleLockOn(player_->camera_);
		lockOnCooltime_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "LockDuration"));
	}
	// スティックでロックオン対象を変更
	if (lockOn_.ExistTarget() && (sThumbR.x != 0 || sThumbR.y != 0) && !lockOnCooltime_.IsActive()) {
		lockOn_.ChangeLockOnTarget(player_->camera_);
		lockOnCooltime_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "LockDuration"));
	}
	if (lockOn_.ExistTarget()) {

	}
	direct = Vector3::Normalize(direct);

	// ダッシュ入力
	bool isQuickBoost = std::holds_alternative<QuickBoostState*>(player_->HorizontalState()->GetVariant());
	if (!isQuickBoost && GameSystem::sKeyConfigManager.GetPlayerKey().quickBoost) {
		if (!player_->GetSystemFacede()->GetEnergy()->CheckQuickBoost()) {
			return;
		}
		if (isQuickBoost) {
			return;
		}
		player_->HorizontalState()->ChangeRequest(PlayerStateLists::kQuickBoost);
		return;
	}

	float velocityDecay = GlobalVariables::GetInstance()->GetValue<float>("Player", "VelocityDecay");
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, velocityDecay);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, velocityDecay);

}
