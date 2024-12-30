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
	Vector3 direct{};
	float playerYaw = player_->GetCamera()->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);
	direct = rotateVector;

	Vector3 sub = Vector3::Normalize({ leftStick_.x,leftStick_.y ,0 });
	if (sub.x != 0.0f || sub.y != 0.0f) {
		sub = Matrix4x4::TransformVector3({ sub.x,0,sub.y }, rotateY);
		player_->worldTransform_.transform_.rotate.y = LwLib::CalculateYawFromVector({ sub.x,0,sub.z });
	}

	const float speed = GlobalVariables::GetInstance()->GetValue<float>("Player", "MoveSpeed");
	//float slowFactor = 0.2f;
	this->activeFrame_++;
	// 入力時間に応じて
	float maxFrame = 120.0f;	// 時間
	// レート(0 ~ 1)(speed/2 ~ speed)
	float rate = LwLib::Normalize(activeFrame_, 0.0f, maxFrame);
	float activeSpeed = Ease::Easing(speed / 2.0f, speed, rate);

	// 入力しているかどうか
	if (direct.x != 0)
	{
		player_->acceleration_.x = (direct.x * GameSystem::GameSpeedFactor() * activeSpeed);
	}
	if (direct.z != 0) {
		player_->acceleration_.z = (direct.z * GameSystem::GameSpeedFactor() * activeSpeed);
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
