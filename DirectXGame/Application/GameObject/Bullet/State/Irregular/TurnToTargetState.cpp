#include "TurnToTargetState.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Bullet/BulletsPaths.h"
#include "Application/GameSystem/GameSystem.h"
#include "../StateMachine/BulletStateMachine.h"
#include "Engine/LwLib/LwEnginePaths.h"

void TurnToTargetState::Enter()
{
	bullet_->SetAccelerate(Vector3::Zero());
	moveDirection_ = bullet_->GetWorldPosition() - bullet_->GetTarget()->worldTransform_.GetWorldPosition();
	turnTimer_.Start(60.0f);
}

void TurnToTargetState::Update(BulletStateMachine& stateMachine)
{
	stateMachine;
	// タイマー処理
	timer_.Update();
	turnTimer_.Update();

	// 旋回のタイミング
	if (turnTimer_.IsEnd()) {
		//Vector3 newVelocity = bullet_->GetVelocity();
		moveDirection_ = bullet_->GetTarget()->worldTransform_.GetWorldPosition() - bullet_->GetWorldPosition();
		moveDirection_.Normalize();
		bullet_->SetVelocity(moveDirection_ * (Vector3::Length(bullet_->GetVelocity()) / 15.0f) * GameSystem::GameSpeedFactor());
		bullet_->SetVelocity(moveDirection_ * (30.0f) * GameSystem::GameSpeedFactor());
		//timer_.Start(120.0f);
	}

	// 加速処理
	float damping = 1.0f / 1.0f;
	// 速度を減衰させた値を設定
	bullet_->SetAccelerate(bullet_->GetVelocity() * damping);
}

void TurnToTargetState::Exit()
{
}
