#include "JumpingState.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"

void JumpingState::Initialize()
{
	nowState_ = this;
	// ジャンプ処理
	JumpActionExecute();
	// 2段目に即入らないように
	jumpCooltime_.Start(15.0f);
}

void JumpingState::Update()
{
	// タイマー
	jumpCooltime_.Update();
	// ステート変更
	if (player_->velocity_.y < 0) {
		stateMachine_->ChangeRequest(PlayerStateLists::kFall);
		return;
	}
	// 落下処理
	const float gravity = GlobalVariables::GetInstance()->GetValue<float>("Player", "FallGravity");
	player_->velocity_.y += gravity * GameSystem::GameSpeedFactor();
}

void JumpingState::Exit()
{
}

void JumpingState::InputHandle()
{
	// ジャンプのクールタイムか
	if (jumpCooltime_.IsActive()) {
		return;
	}
	// 入力で切り替える受付
	if (GameSystem::sKeyConfigManager.GetPlayerKey().jump && !player_->IsDoubleJump()) {
		player_->SetIsDoubleJump(true);
		JumpActionExecute();
	}

}

void JumpingState::JumpActionExecute()
{
	float jumpPower = 0.0f;
	if (!player_->IsDoubleJump()) {
		jumpPower = (GlobalVariables::GetInstance()->GetValue<float>("Player", "FirstJumpPower"));
	}
	else {
		jumpPower = (GlobalVariables::GetInstance()->GetValue<float>("Player", "SecondJumpPower"));
	}

	// 速度計算
	player_->velocity_.y += jumpPower * GameSystem::GameSpeedFactor();
	// 座標更新
	player_->worldTransform_.transform_.translate += player_->velocity_;

}
