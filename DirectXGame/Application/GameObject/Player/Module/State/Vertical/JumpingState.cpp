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
	float gravity = GlobalVariables::GetInstance()->GetValue<float>("Player", "FallGravity");
	player_->velocity_.y += gravity * GameSystem::GameSpeedFactor();
}

void JumpingState::Exit()
{
}

void JumpingState::InputHandle()
{
	float energyRatio = player_->GetSystemFacede()->GetEnergy()->GetEnergyRatio();
	if (GameSystem::sKeyConfigManager.GetPlayerKey().pressJump && energyRatio >= 0.5f)
	{
		player_->VerticalState()->ChangeRequest(PlayerStateLists::kAssending);
	}

	if (jumpCooltime_.IsActive()) {
		return;
	}
	if (GameSystem::sKeyConfigManager.GetPlayerKey().jump && !player_->isDoubleJump_) {
		player_->isDoubleJump_ = true;
		JumpActionExecute();
	}

}

void JumpingState::JumpActionExecute()
{
	float jumpPower = 0.0f;
	if (!player_->isDoubleJump_) {
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
