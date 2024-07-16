#include "QuickBoostState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/PostEffect/PostEffectRender.h"

void QuickBoostState::Initialize()
{
	IPlayerState::Initialize();
	// カメラの回転に方向を適応
	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	player_->worldTransform_.transform_.rotate.y = player_->camera_->transform_.rotate.y;
	Vector3 direct = {};
	leftStick_ = input_->XGetLeftJoystick();
	direct = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);

	float dashPower = 100.0f * GameSystem::GameSpeedFactor();
	player_->velocity_.x = direct.x * dashPower;
	player_->velocity_.z = direct.z * dashPower;
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;

}

void QuickBoostState::Update()
{
	// 減速率
	float slowFactor = 0.2f;

	// 減速処理
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);

	if (player_->velocity_.x == 0.0f && player_->velocity_.z == 0.0f) {
		stateMachine_->ChangeRequest(HorizontalStates::kIdle);
		return;
	}

}

void QuickBoostState::Exit()
{
	PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
}

void QuickBoostState::InputHandle()
{
	IPlayerState::InputHandle();

}
