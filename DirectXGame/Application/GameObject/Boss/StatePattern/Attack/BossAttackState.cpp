#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"

void BossState::AttackState::Initialize()
{
	boss_->SetNowVariantState(this);

	fireCooltime_ = 15.0f;
	changeTimer_.Start(90.0f);
	fireTimer_.Start(fireCooltime_);
	// 攻撃パターンのランダム
	uint32_t randM = LwLib::GetRandomValue(0, 2);
	if (randM == 0) {
		pattern_ = ShotPattern::kPredictive;
	}
	else if (randM == 1) {
		pattern_ = ShotPattern::kStraight;
	}
	else {
		pattern_ = ShotPattern::kSpread;
	}
	pattern_ = ShotPattern::kRadialFire;
	// 角度
	rotateAngle_ = 0.01f;

	//---弾の情報---//
	// 速さ
	bulletSpeed_ = 50.0f;
	// サイズ
	bulletScale_ = 0.4f;
	// 進む方向
	bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
}

void BossState::AttackState::Update()
{
	if (!fireTimer_.IsActive()) {
		GenerateProcess();
		fireTimer_.Start(fireCooltime_);
	}
	// 射撃タイマー
	fireTimer_.Update();

	TimerUpdate(this);
}

void BossState::AttackState::Exit()
{
	boss_->SetPrevVariantState(this);

}

void BossState::AttackState::SimpleAttack(const Vector3& position)
{
	position;


}

void BossState::AttackState::LockAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);
}

void BossState::AttackState::StraightAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);
}

void BossState::AttackState::SpreadAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 直線
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);

	Vector2 bulletDirect = { bulletDirect_.x,bulletDirect_.z };
	float rotValue = LwLib::GetRandomValue(0.1f, 0.5f);
	Matrix3x3 leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
	Matrix3x3 rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
	// 1
	// 左回転
	bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, leftRotateMat);
	Vector3 newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	// 右回転
	bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, rightRotateMat);
	newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);

	// 2
	rotValue += 0.1f;
	leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
	rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
	// 左回転
	bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, leftRotateMat);
	newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	// 右回転
	bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, rightRotateMat);
	newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);

	// 3
	rotValue += 0.1f;
	leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
	rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
	// 左回転
	bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, leftRotateMat);
	newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	// 右回転
	bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, rightRotateMat);
	newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);

}

void BossState::AttackState::RadialFireAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 直線
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);

	Vector2 bulletDirect = { bulletDirect_.x,bulletDirect_.z };
	float rotValue = LwLib::GetRandomValue(0.1f, 0.5f);
	rotValue = 0.5f;
	float addValue = 1.0f;
	Matrix3x3 leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
	Matrix3x3 rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
	int max = 7;
	for (int i = 0; i < max; ++i) {
		rotValue += addValue;
		leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
		rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
		// 左回転
		bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, leftRotateMat);
		Vector3 newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
		// 右回転
		bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, rightRotateMat);
		newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	}


	//// 2
	//rotValue += addValue;
	//leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
	//rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
	//// 左回転
	//bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, leftRotateMat);
	//newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	//boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	//// 右回転
	//bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, rightRotateMat);
	//newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	//boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);

	//// 3
	//rotValue += addValue;
	//leftRotateMat = Matrix3x3::MakeRotateMatrix(-rotValue);
	//rightRotateMat = Matrix3x3::MakeRotateMatrix(rotValue);
	//// 左回転
	//bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, leftRotateMat);
	//newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	//boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	//// 右回転
	//bulletDirect = Matrix3x3::Transform({ bulletDirect_.x,bulletDirect_.z }, rightRotateMat);
	//newDirect = { bulletDirect.x,bulletDirect_.y,bulletDirect.y };
	//boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
}

void BossState::AttackState::GenerateProcess()
{
	switch (pattern_)
	{
	case BossState::AttackState::ShotPattern::kPredictive:
		LockAttack();
		break;
	case BossState::AttackState::ShotPattern::kStraight:
		StraightAttack();
		break;
	case BossState::AttackState::ShotPattern::kSpread:
		SpreadAttack();
		break;
	case BossState::AttackState::ShotPattern::kRadialFire:
		RadialFireAttack();
		break;
	default:
		break;
	}
}
