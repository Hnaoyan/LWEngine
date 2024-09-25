#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"

void BossState::AttackState::Initialize()
{
	boss_->SetNowVariantState(this);
	preActionTimer_.Start(60.0f);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, 55.0f);

	// 攻撃パターンのランダム
	uint32_t randM = LwLib::GetRandomValue(0, 3);
	if (randM == 0) {
		pattern_ = ShotPattern::kPredictive;
		fireCooltime_ = 10.0f;
	}
	else if (randM == 1) {
		pattern_ = ShotPattern::kStraight;
		fireCooltime_ = 15.0f;
	}
	else if(randM == 2){
		pattern_ = ShotPattern::kSpread;
		fireCooltime_ = 20.0f;
	}
	else {
		pattern_ = ShotPattern::kRadialFire;
		fireCooltime_ = 20.0f;
	}

	//pattern_ = ShotPattern::kSpread;

	// 初期の角度
	startRotate_ = boss_->worldTransform_.transform_.rotate.y;

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
	// 前の待機
	preActionTimer_.Update();
	// 待機終了タイミング
	if (preActionTimer_.IsEnd()) {
		fireTimer_.Start(fireCooltime_);
		changeTimer_.Start(90.0f);
	}

	// クールタイム
	if (fireTimer_.IsEnd()) {
		GenerateProcess();
		fireTimer_.Start(fireCooltime_);
	}
	// 射撃の処理中
	if (fireTimer_.IsActive()) {
		if (pattern_ == ShotPattern::kRadialFire) {
			boss_->worldTransform_.transform_.rotate.y = LwLib::Lerp(startRotate_, startRotate_ + 6.14f, changeTimer_.GetElapsedFrame());
		}
	}
	// 射撃タイマー
	fireTimer_.Update();

	TimerUpdate(this);
}

void BossState::AttackState::Exit()
{
	boss_->SetPrevVariantState(this);
	boss_->worldTransform_.transform_.rotate.y = startRotate_;
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, 30.0f);
}

void BossState::AttackState::SimpleAttack(const Vector3& position)
{
	position;


}

void BossState::AttackState::Setting(float stateTimer, ShotPattern pattern)
{
	pattern_ = pattern;
	changeTimer_.Start(stateTimer);
}

void BossState::AttackState::LockAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
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
	float rotValue = LwLib::GetRandomValue(0.1f, 0.5f);
	Vector3 newDirect{};
	// 1
	for (int i = 0; i < 3; ++i) {
		// 2
		rotValue += 0.1f;
		// 左回転
		newDirect = LwLib::RotateXZVector(bulletDirect_, -rotValue);
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
		// 右回転
		newDirect = LwLib::RotateXZVector(bulletDirect_, rotValue);
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	}

}

void BossState::AttackState::RadialFireAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	pos.scale *= 3.5f;
	// 直線
	boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, bulletDirect_, bulletSpeed_);

	Vector2 bulletDirect = { bulletDirect_.x,bulletDirect_.z };
	Matrix4x4 yawMatrix = Matrix4x4::MakeRotateYMatrix(boss_->worldTransform_.transform_.rotate.y);
	Vector3 direct = Matrix4x4::TransformVector3(bulletDirect_, yawMatrix);

	float rotValue = LwLib::GetRandomValue(0.1f, 0.5f);
	rotValue = 0.5f;
	float addValue = 1.0f;
	int max = 7;
	for (int i = 0; i < max; ++i) {
		rotValue += addValue;
		// 左回転
		Vector3 newDirect = LwLib::RotateXZVector({ direct.x, 0.0f,direct.z }, -rotValue);
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
		// 右回転
		newDirect = LwLib::RotateXZVector({ direct.x, 0.0f,direct.z }, rotValue);
		boss_->GetBulletManager()->GetBeginCluster()->AddBullet(pos, newDirect, bulletSpeed_);
	}
}

void BossState::AttackState::GenerateProcess()
{
	// 弾の方向ベクトル設定
	Vector3 playerPos = boss_->GetPlayer()->worldTransform_.GetWorldPosition();
	playerPos.y += 0.5f;
	bulletDirect_ = Vector3::Normalize(playerPos - boss_->worldTransform_.GetWorldPosition());

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
