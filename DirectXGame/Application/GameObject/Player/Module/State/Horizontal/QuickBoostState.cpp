#include "QuickBoostState.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/PostEffect/PostEffectRender.h"

void QuickBoostState::Initialize()
{
	nowState_ = this;

	InputHandle();
	// 方向設定
	Vector3 direct{};
	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);
	direct = rotateVector;

	float dashPower = 40.0f;
	dashVelocity_.x = direct.x * dashPower;
	dashVelocity_.z = direct.z * dashPower;
	changeTimer_.Start(40.0f);

	player_->GetEnergyManager()->QuickBoostDecre();

}

void QuickBoostState::Update()
{
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
	changeTimer_.Update();

	dashVelocity_.x = LwLib::Lerp(dashVelocity_.x, 0, changeTimer_.GetElapsedFrame());
	dashVelocity_.z = LwLib::Lerp(dashVelocity_.z, 0, changeTimer_.GetElapsedFrame());

	player_->velocity_.x += dashVelocity_.x * GameSystem::GameSpeedFactor();
	player_->velocity_.z += dashVelocity_.z * GameSystem::GameSpeedFactor();

	if (dashVelocity_.x == 0.0f && dashVelocity_.z == 0.0f) {
		if (leftStick_.x != 0 || leftStick_.y != 0) {
			stateManager_->ChangeRequest(StateManager::kMove, StateManager::kHorizontal);
		}
		else {
			stateManager_->ChangeRequest(StateManager::kIdleHorizontal, StateManager::kHorizontal);
		}
		return;
	}
}

void QuickBoostState::Exit()
{
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
	player_->quickBoostCoolTime_.Start(45.0f);

	player_->GetAnimationManager()->Reset();
}
	
void QuickBoostState::InputHandle()
{
	IPlayerState::InputHandle();

}
