#include "OperationManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../Player.h"
#include "../../Bullet/BulletManager.h"
#include "Application/GameSystem/GameSystem.h"

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
	//float speed = 4.0f;
	Vector3 direct = {};
	Vector2 sThumbL = input_->XGetLeftJoystick();
	Vector2 sThumbR = input_->XGetRightJoystick();
	// コントローラー操作
	// 方向取得
	direct = { sThumbL.x,sThumbL.y ,0 };
	// 射撃入力
	if (GameSystem::sPlayerKey.keyConfigs_.shot && !shotTimer_.IsActive()) {
		Vector3 velocity = Vector3::Normalize(aimManager_.GetWorldPosition() - player_->worldTransform_.GetWorldPosition());
		float speedValue = 30.0f;

		if (lockOn_.ExistTarget()) {
			speedValue *= 2.0f;
		}

		velocity *= speedValue;
		EulerTransform transform{};
		transform.scale = { 1.0f,1.0f,1.0f };
		transform.translate = player_->worldTransform_.GetWorldPosition();
		bulletManager_->FindCluster("NormalBullet")->AddBullet(transform, velocity);
		shotTimer_.Start(30.0f);
	}

	// カメラの処理
	if (GameSystem::sPlayerKey.keyConfigs_.lockon && !lockOnCooltime_.IsActive()) {
		lockOn_.ToggleLockOn(player_->camera_);
		lockOnCooltime_.Start(20.0f);
	}
	// スティックでロックオン対象を変更
	if (lockOn_.ExistTarget() && (sThumbR.x != 0 || sThumbR.y != 0) && !lockOnCooltime_.IsActive()) {
		lockOn_.ChangeLockOnTarget(player_->camera_);
		lockOnCooltime_.Start(20.0f);
	}

	direct = Vector3::Normalize(direct);

	float slowFactor = 0.2f;
	bool isQucikBoost = std::holds_alternative<QuickBoostState*>(player_->GetHorizontalState()->GetNowState());
	if (!isQucikBoost && GameSystem::sPlayerKey.keyConfigs_.quickBoost && !player_->quickBoostCoolTime_.IsActive()) {
		if (!player_->GetSystemFacede()->GetEnergy()->CheckQuickBoost()) {
			return;
		}
		player_->GetStateManager()->ChangeRequest(StateManager::kQuickBoost, StateManager::kHorizontal);
		return;
	}

	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

}
