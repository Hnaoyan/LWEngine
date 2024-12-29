#include "QuickBoostState.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/Effect/PostEffectManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void QuickBoostState::Initialize()
{
	nowState_ = this;

	InputHandle();
	// 方向設定
	Vector3 direct{};
	float playerYaw = player_->GetCamera()->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);
	direct = rotateVector;

	const float dashPower = GlobalVariables::GetInstance()->GetValue<float>("Player", "DashPower");
	dashVelocity_.x = direct.x * dashPower;
	dashVelocity_.z = direct.z * dashPower;
	changeTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "QuickBoostEndTime"));
	// ゲージ減少
	player_->GetSystemFacede()->GetEnergy()->QuickBoostDecre();
	// ジャスト回避受付開始
	player_->GetSystemFacede()->GetDudgeManager()->DodgeExcept();

	// ダッシュエフェクト
	PostEffectManager::sDashEffect.Initialize();
}

void QuickBoostState::Update()
{
	changeTimer_.Update();
	// 減速処理
	dashVelocity_.x = LwLib::Lerp(dashVelocity_.x, 0, changeTimer_.GetElapsedFrame());
	dashVelocity_.z = LwLib::Lerp(dashVelocity_.z, 0, changeTimer_.GetElapsedFrame());
	// フレーム内の速度計算
	player_->acceleration_.x = dashVelocity_.x * GameSystem::GameSpeedFactor();
	player_->acceleration_.z = dashVelocity_.z * GameSystem::GameSpeedFactor();

	// 変更処理
	if (dashVelocity_.x == 0.0f && dashVelocity_.z == 0.0f) {
		if (leftStick_.x != 0 || leftStick_.y != 0) {
			stateMachine_->ChangeRequest(PlayerStateLists::kMove);
		}
		else {
			stateMachine_->ChangeRequest(PlayerStateLists::kIdleHorizontal);
		}
		return;
	}
}

void QuickBoostState::Exit()
{
	PostEffectManager::sDashEffect.Finalize();

	player_->GetSystemFacede()->GetAnimation()->Reset();
	player_->GetOperation()->SetCooltime(GlobalVariables::GetInstance()->GetValue<float>("Player", "DashCooltime"));
}
	
void QuickBoostState::InputHandle()
{
	IPlayerState::InputHandle();
}
