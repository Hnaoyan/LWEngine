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
}

void OparationManager::Update()
{
	// 入力
	InputUpdate();
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
	if (GameSystem::sKeyConfigManager.GetPlayerKey().shot) {
		// 武器マネージャにリクエストを送るようにする（メモ
		player_->GetWeaponManager()->AttackReception();
	}
	//// 追従（強化弾として扱う
	//else if (GameSystem::sKeyConfigManager.GetPlayerKey().homingShot && !shotTimer_.IsActive()) {
	//	player_->GetSystemFacede()->GetShootingManager()->TrackingFire();
	//	shotTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "ShotDuration"));
	//}

	// カメラの処理
	if (GameSystem::sKeyConfigManager.GetPlayerKey().lockon) {
		player_->GetWeaponManager()->LockOnReception();
	}
	//// スティックでロックオン対象を変更
	//if (lockOn_.ExistTarget() && (sThumbR.x != 0 || sThumbR.y != 0) && !lockOnCooltime_.IsActive()) {
	//	lockOn_.ChangeLockOnTarget(player_->GetCamera());
	//	lockOnCooltime_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "LockDuration"));
	//}
	direct = Vector3::Normalize(direct);
	// 加速度による速度計算
	player_->velocity_ += player_->acceleration_;

	// 速度の計算
	const float velocityDecay = GlobalVariables::GetInstance()->GetValue<float>("Player", "VelocityDecay");
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, velocityDecay);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, velocityDecay);

	// 長押し確認
	longDashTimer_.Update();
	longDashCoolTimer_.Update();
	// ダッシュ入力
	bool isQuickBoost = std::holds_alternative<QuickBoostState*>(player_->HorizontalState()->GetVariant()) || std::holds_alternative<BoostState*>(player_->HorizontalState()->GetVariant());
	bool isLeftStick = (sThumbL.x != 0.0f || sThumbL.y != 0.0f);
	// ダッシュ中かクールタイムなら早期
	if ((isQuickBoost) || longDashCoolTimer_.IsActive()) {
		return;
	}

	if (isLeftStick) {
		// 長めのダッシュの場合
		if (longDashTimer_.IsEnd()) {
			player_->HorizontalState()->ChangeRequest(PlayerStateLists::kBoost);
			return;
		}
		// 小ダッシュの場合
		if (!GameSystem::sKeyConfigManager.GetPlayerKey().quickBoost && longDashTimer_.IsActive()) {
			player_->HorizontalState()->ChangeRequest(PlayerStateLists::kQuickBoost);
			return;
		}
	}

	// ダッシュ入力の受付
	if (GameSystem::sKeyConfigManager.GetPlayerKey().quickBoost) {
		// エネルギー側でこの入力が可能か
		bool isEnergy = !player_->GetSystemFacede()->GetEnergy()->IsOverheat();
		// ロングダッシュにするかどうか
		if (isEnergy && !longDashTimer_.IsActive()) {
			const float dashFrame = 12.0f;
			longDashTimer_.Start(dashFrame);
		}
	}


}
