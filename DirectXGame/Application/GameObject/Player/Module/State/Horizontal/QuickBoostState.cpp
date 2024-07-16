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
	//PostEffectRender::sPostEffect = Pipeline::PostEffectType::kRadialBlur;
	blurTimer_.Start(15.0f);
}

void QuickBoostState::Update()
{
	// 速度の値で終了判定
	if (player_->velocity_.x == 0.0f && player_->velocity_.z == 0.0f) {
		if (isLeftStickActive_) {
			if (isBoost_) {
				stateMachine_->ChangeRequest(HorizontalStates::kBoost);
			}
			else {
				stateMachine_->ChangeRequest(HorizontalStates::kMove);
			}
		}
		else {
			stateMachine_->ChangeRequest(HorizontalStates::kIdle);
		}
		return;
	}
	if (blurTimer_.isActive_) {
		blurTimer_.Update();
		if (blurTimer_.isEnd_) {
			PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
		}
	}
	// 減速率
	float slowFactor = 0.175f;

	// 減速処理
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor, 0.001f);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor, 0.001f);

}

void QuickBoostState::Exit()
{
	player_->velocity_ = {};
}

void QuickBoostState::InputHandle()
{
	IPlayerState::InputHandle();

}
