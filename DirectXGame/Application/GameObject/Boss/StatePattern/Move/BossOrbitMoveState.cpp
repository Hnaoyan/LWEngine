#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::OrbitMoveState::Initialize()
{
}

void BossState::OrbitMoveState::Update()
{
	boss_->GetDecider()->StateDecide(this);
}

void BossState::OrbitMoveState::Exit()
{
	boss_->SetPrevVariantState(this);
}

void BossState::OrbitMoveState::GenerateMovePoint(float length, QuaterRotationPattern pattern)
{
	controlPoint.LFront = { -length, -length };
	controlPoint.LBack = { -length,length };
	controlPoint.RFront = { length,-length };
	controlPoint.RBack = { length,length };

	Vector2 front = { 0,-length };
	Vector2 back = { 0.0f,length };
	Vector2 right = { length,0.0f };
	Vector2 left = { -length,0.0f };

	startPosition_ = boss_->worldTransform_.GetWorldPosition();
	// 右奥
	if (startPosition_.x >= 0 && startPosition_.z >= 0) {

	}
	// 左奥
	else if (startPosition_.x <= 0 && startPosition_.z >= 0) {

	}
	// 右前
	else if (startPosition_.x >= 0 && startPosition_.z <= 0) {

	}
	// 左前
	else if (startPosition_.x <= 0 && startPosition_.z <= 0) {

	}

	switch (pattern)
	{
	case BossState::OrbitMoveState::QuaterRotationPattern::kFirst:

		break;
	case BossState::OrbitMoveState::QuaterRotationPattern::kSecond:

		break;
	case BossState::OrbitMoveState::QuaterRotationPattern::kThird:

		break;
	case BossState::OrbitMoveState::QuaterRotationPattern::kFourth:

		break;
	default:
		break;
	}
}