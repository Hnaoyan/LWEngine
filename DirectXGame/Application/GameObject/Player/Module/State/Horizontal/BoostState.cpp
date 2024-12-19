#include "BoostState.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameObject/Bullet/BulletManager.h"
#include "Application/GameSystem/Effect/PostEffectManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void BoostState::Initialize()
{
	nowState_ = this;
	// 入力
	InputHandle();
	// 方向設定
	Vector3 direct{};
	float playerYaw = player_->GetCamera()->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);
	direct = rotateVector;

	// 速度の計算
	const float dashPower = GlobalVariables::GetInstance()->GetValue<float>("Player", "BoostDashPower");
	dashVelocity_.x = direct.x * dashPower;
	dashVelocity_.z = direct.z * dashPower;
	changeTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("Player", "BoostEndTime"));
	// ゲージ減少
	player_->GetSystemFacede()->GetEnergy()->QuickBoostDecre();
	// ジャスト回避受付開始
	player_->GetSystemFacede()->GetDudgeManager()->DodgeExcept();
	PostEffectManager::sDashEffect.Initialize();
}

void BoostState::Update()
{
	// 変更までの時間
	changeTimer_.Update();

	// 速度の計算
	dashVelocity_.x = LwLib::Lerp(dashVelocity_.x, 0, changeTimer_.GetElapsedFrame());
	dashVelocity_.z = LwLib::Lerp(dashVelocity_.z, 0, changeTimer_.GetElapsedFrame());
	player_->velocity_.x += dashVelocity_.x * GameSystem::GameSpeedFactor();
	player_->velocity_.z += dashVelocity_.z * GameSystem::GameSpeedFactor();

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

void BoostState::Exit()
{
	player_->GetSystemFacede()->GetAnimation()->Reset();
	player_->GetOperation()->SetCooltime(GlobalVariables::GetInstance()->GetValue<float>("Player", "DashCooltime"));
}

void BoostState::InputHandle()
{
	IPlayerState::InputHandle();
}
