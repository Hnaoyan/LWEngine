#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::MoveState::Initialize()
{
	changeTimer_.Start(150.0f);
	if (boss_->worldTransform_.GetWorldPosition().x > 0) {
		isLeft_ = true;
	}
	else {
		isLeft_ = false;
	}
	boss_->worldTransform_.transform_.rotate.y = 0.0f;
	boss_->worldTransform_.UpdateMatrix();
}

void BossState::MoveState::Update()
{
	float speed = 0.5f;
	// とりあえずの左右移動
	if (isLeft_) {
		boss_->worldTransform_.transform_.translate.x -= speed;
	}
	else {
		boss_->worldTransform_.transform_.translate.x += speed;
	}

	TimerUpdate(this);
}

void BossState::MoveState::Exit()
{
	boss_->SetPrevVariantState(this);
	isLeft_ = false;
}

void BossState::MoveState::MoveSelect(const Vector3& playerPosition)
{
	float yPosition = boss_->worldTransform_.GetWorldPosition().y;
	std::vector<Vector3> points;
	// 右奥
	points.push_back({ kMaxPoint,yPosition,kMaxPoint });
	// 左奥
	points.push_back({ -kMaxPoint,yPosition,kMaxPoint });
	// 右手前
	points.push_back({ kMaxPoint,yPosition,-kMaxPoint });
	// 左手前
	points.push_back({ -kMaxPoint,yPosition,-kMaxPoint });

	// 右前
	if (playerPosition.x > 0 && playerPosition.z > 0) {
		//boss_->worldTransform_.transform_.translate = points[3];
	}
	// 左前
	else if (playerPosition.x <= 0 && playerPosition.z > 0) {
		//boss_->worldTransform_.transform_.translate = points[2];
	}
	// 右後ろ
	else if (playerPosition.x > 0 && playerPosition.z <= 0) {
		//boss_->worldTransform_.transform_.translate = points[1];
	}
	// 左後ろ
	else if (playerPosition.x <= 0 && playerPosition.z <= 0) {
		//boss_->worldTransform_.transform_.translate = points[0];
	}

}

void BossState::MoveState::TestProcess()
{
	//changeTimer_.Start(300.0f);
}
