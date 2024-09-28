#include "MovingState.h"
#include "Application/GameObject/Player/Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/LwLib/LwEngineLib.h"

void MovingState::Initialize()
{
	nowState_ = this;
}

void MovingState::Update()
{
	// 方向ベクトル
	Vector3 direct{};
	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ leftStick_.x,0,leftStick_.y }, rotateY);
	direct = rotateVector;

	Vector3 sub = Vector3::Normalize({ leftStick_.x,leftStick_.y ,0 });
	if (sub.x != 0.0f || sub.y != 0.0f) {
		sub = Matrix4x4::TransformVector3({ sub.x,0,sub.y }, rotateY);
		player_->worldTransform_.transform_.rotate.y = LwLib::CalculateYawFromVector({ sub.x,0,sub.z });
	}

	float speed = 4.0f;
	//float slowFactor = 0.2f;

	// 入力しているかどうか
	if (direct.x != 0)
	{
		player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
	}
	if (direct.z != 0) {
		player_->velocity_.z += (direct.z * GameSystem::GameSpeedFactor() * speed);
	}

	if (!isLeftStickActive_) {
		stateManager_->ChangeRequest(StateManager::kIdleHorizontal);
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
