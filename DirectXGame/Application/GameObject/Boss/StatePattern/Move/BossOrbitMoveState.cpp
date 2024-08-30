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
		fireTimer_.Start(2.5f);
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
			fireTimer_.Start(2.5f);
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
	Player* player = boss_->GetPlayer();
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 弾の方向設定
	Vector3 playerPos = player->worldTransform_.GetWorldPosition();
	Vector3 playerMove = player->prevPosition_ - player->worldTransform_.GetWorldPosition();
	// 予測位置を計算
	float predictionTime = 10.0f; // ミサイルが向かう予測時間（調整可能）

	if (playerMove.x == 0.0f && playerMove.y == 0.0f && playerMove.z == 0.0f) {
		playerPos = playerPos;
	}
	else {
		playerPos = playerPos + (playerMove * predictionTime);
	}
	if (playerPos.y < 4.0f) {
		playerPos.y += 0.5f;
	}
	bulletDirect_ = Vector3::Normalize(playerPos - boss_->worldTransform_.GetWorldPosition());
	
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);
}
