#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/Ease/Ease.h"
#include "Engine/LwLib/LwEngineLib.h"

void BossState::OrbitMoveState::Initialize()
{
	boss_->SetNowVariantState(this);
	startPosition_ = boss_->worldTransform_.GetWorldPosition();
	changeTimer_.Start(150.0f);

	if (boss_->GetHealth()->GetHPRatio() < 0.75f) {
		isAttack_ = true;
		fireTimer_.Start(5.0f);
	}
	else {
		isAttack_ = false;
	}

	//---弾の情報---//
	// 速さ
	bulletSpeed_ = 50.0f;
	// サイズ
	bulletScale_ = 0.4f;
	// 進む方向
	bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
}

void BossState::OrbitMoveState::Update()
{
	if (isAttack_) {
		fireTimer_.Update();
		if (!fireTimer_.IsActive()) {
			fireTimer_.Start(5.0f);
			LockOnAttack();
		}

	}
	changeTimer_.Update();
	RotateUpdate();
	GenerateMovePoint(100.0f, OrbitMoveState::QuaterRotationPattern::kFirst);
	if (!changeTimer_.IsActive()) {
		boss_->GetDecider()->StateDecide(this);
	}

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

	// 右奥
	if (startPosition_.x >= 0 && startPosition_.z >= 0) {
		Vector2 bezier = LwLib::Curve::BezierCurve({ startPosition_.x,startPosition_.z }, controlPoint.RFront, front, changeTimer_.GetElapsedFrame());
		boss_->worldTransform_.transform_.translate.x = bezier.x;
		boss_->worldTransform_.transform_.translate.z = bezier.y;
	}
	// 左奥
	else if (startPosition_.x <= 0 && startPosition_.z >= 0) {
		Vector2 bezier = LwLib::Curve::BezierCurve({ startPosition_.x,startPosition_.z }, controlPoint.RBack, right, changeTimer_.GetElapsedFrame());
		boss_->worldTransform_.transform_.translate.x = bezier.x;
		boss_->worldTransform_.transform_.translate.z = bezier.y;
	}
	// 右前
	else if (startPosition_.x >= 0 && startPosition_.z <= 0) {
		Vector2 bezier = LwLib::Curve::BezierCurve({ startPosition_.x,startPosition_.z }, controlPoint.LFront, left, changeTimer_.GetElapsedFrame());
		boss_->worldTransform_.transform_.translate.x = bezier.x;
		boss_->worldTransform_.transform_.translate.z = bezier.y;
	}
	// 左前
	else if (startPosition_.x <= 0 && startPosition_.z <= 0) {
		Vector2 bezier = LwLib::Curve::BezierCurve({ startPosition_.x,startPosition_.z }, controlPoint.LBack, back, changeTimer_.GetElapsedFrame());
		boss_->worldTransform_.transform_.translate.x = bezier.x;
		boss_->worldTransform_.transform_.translate.z = bezier.y;
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

void BossState::OrbitMoveState::LockOnAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 弾の方向設定
	Vector3 playerPos = boss_->GetPlayer()->worldTransform_.GetWorldPosition();
	playerPos.y += 0.5f;
	bulletDirect_ = Vector3::Normalize(playerPos - boss_->worldTransform_.GetWorldPosition());
	
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);
}
