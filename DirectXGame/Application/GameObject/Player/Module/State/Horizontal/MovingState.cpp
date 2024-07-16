#include "MovingState.h"
#include "../StateMachine.h"
#include "../../../Player.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/LwLib/LwEngineLib.h"

void MovingState::Initialize()
{
	IPlayerState::Initialize();
}

void MovingState::Update()
{
	if (!isLeftStickActive_) {
		stateMachine_->ChangeRequest(HorizontalStates::kIdle);
		return;
	}

	if (isBoost_) {
		stateMachine_->ChangeRequest(HorizontalStates::kBoost);
		return;
	}
	float speed = 3.0f;
	Vector3 direct = {};
	Vector2 sThumbL = input_->XGetLeftJoystick();
	direct = { sThumbL.x,sThumbL.y ,0 };
	direct = Vector3::Normalize(direct);

	// カメラの回転に方向を適応
	float playerYaw = player_->camera_->transform_.rotate.y;
	Matrix4x4 rotateY = Matrix4x4::MakeRotateYMatrix(playerYaw);
	player_->worldTransform_.transform_.rotate.y = player_->camera_->transform_.rotate.y;
	direct = Matrix4x4::TransformVector3({ direct.x,0,direct.y }, rotateY);

	// 入力しているかどうか
	float slowFactor = 0.2f;
	// 速度処理
	player_->velocity_.x += (direct.x * GameSystem::GameSpeedFactor() * speed);
	player_->velocity_.z += (direct.z * GameSystem::GameSpeedFactor() * speed);
	// 減速処理
	player_->velocity_.x = LwLib::Lerp(player_->velocity_.x, 0, slowFactor);
	player_->velocity_.z = LwLib::Lerp(player_->velocity_.z, 0, slowFactor);


}

void MovingState::Exit()
{
}

void MovingState::InputHandle()
{
	IPlayerState::InputHandle();



}
