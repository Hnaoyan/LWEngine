#include "MovingState.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/LwLib/LwLibrary.h"

void MovingState::Initialize()
{
	nowState_ = this;
	activeFrame_ = 0.0f;
}

void MovingState::Update()
{
	// 方向ベクトル
	float playerYaw = player_->GetCamera()->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);

	if (leftStick_.x != 0.0f || leftStick_.y != 0.0f) {
		player_->worldTransform_.transform_.rotate.y = LwLib::ParentRotateY(Vector3(leftStick_.x, leftStick_.y, 0.0f), player_->GetCamera()->transform_.rotate.y);
	}

	const float speed = GlobalVariables::GetInstance()->GetValue<float>("Player", "MoveSpeed");
	//float slowFactor = 0.2f;
	this->activeFrame_++;
	// 入力時間に応じて
	float maxFrame = 75.0f;	// 時間
	// レート(0 ~ 1)(speed/2 ~ speed)
	float rate = LwLib::Normalize(activeFrame_, 0.0f, maxFrame);
	float activeSpeed = Ease::Easing(speed * (3.0f / 4.0f), speed, rate);

	// 入力しているかどうか
	if (rotateVector.x != 0)
	{
		player_->acceleration_.x = (rotateVector.x * GameSystem::GameSpeedFactor() * activeSpeed);
	}
	if (rotateVector.z != 0) {
		player_->acceleration_.z = (rotateVector.z * GameSystem::GameSpeedFactor() * activeSpeed);
	}

	if (!isLeftStickActive_) {
		stateMachine_->ChangeRequest(PlayerStateLists::kIdleHorizontal);
		return;
	}
}

void MovingState::Exit()
{
	player_->GetSystemFacede()->GetAnimation()->Reset();
}

void MovingState::InputHandle()
{
	IPlayerState::InputHandle();
}
