#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void BossState::AttackState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	boss_->SetNowVariantState(this);
	float actionDuration = 60.0f;
	preActionTimer_.Start(actionDuration);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// 攻撃パターンのランダム
	//uint32_t pattern = LwLib::GetRandomValue(0, 3);
	//float coolTime = 10.0f;
	//if (pattern == 0) {
	//	pattern_ = ShotPattern::kPredictive;
	//	fireCooltime_ = coolTime;
	//}
	//else if (pattern == 1) {
	//	pattern_ = ShotPattern::kStraight;
	//	coolTime += 5.0f;
	//	fireCooltime_ = coolTime;
	//}
	//else if(pattern == 2){
	//	pattern_ = ShotPattern::kSpread;
	//	coolTime += 10.0f;
	//	fireCooltime_ = coolTime;
	//}
	//else {
	//	pattern_ = ShotPattern::kRadialFire;
	//	coolTime += 15.0f;
	//	fireCooltime_ = coolTime;
	//}
	pattern_ = ShotPattern::kRadialFire;
	// 初期の角度
	startRotate_ = boss_->worldTransform_.transform_.rotate.y;

	//---弾の情報---//
	// 速さ
	bulletSpeed_ = 50.0f;
	// サイズ
	bulletScale_ = 1.0f;
	// 進む方向
	bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
	// クールタイム
	fireCooltime_ = 5.0f;
	// 初期化
	IState::Initialize();
}

void BossState::AttackState::Update()
{
	// 前の待機
	preActionTimer_.Update();
	// 待機終了タイミング
	if (preActionTimer_.IsEnd()) {
		fireTimer_.Start(fireCooltime_);
		float changeFrame = 90.0f;
		changeTimer_.Start(changeFrame);
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
	GlobalVariables* global = GlobalVariables::GetInstance();
	
	boss_->SetPrevVariantState(this);
	boss_->worldTransform_.transform_.rotate.y = startRotate_;
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, global->GetValue<float>("BossAnimation", "CloseFrame"));
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
	//pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	GenerateBullet(bulletDirect_, bulletSpeed_);
}

void BossState::AttackState::StraightAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	//pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	GenerateBullet(bulletDirect_, bulletSpeed_);
}

void BossState::AttackState::SpreadAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	//pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 直線
	GenerateBullet(bulletDirect_, bulletSpeed_);
	float rotValue = LwLib::GetRandomValue(0.1f, 0.5f);
	Vector3 newDirect{};
	// 1
	for (int i = 0; i < 3; ++i) {
		// 2
		rotValue += 0.1f;
		// 左回転
		newDirect = LwLib::RotateXZVector(bulletDirect_, -rotValue);
		GenerateBullet(newDirect, bulletSpeed_);
		// 右回転
		newDirect = LwLib::RotateXZVector(bulletDirect_, rotValue);
		GenerateBullet(newDirect, bulletSpeed_);
	}

}

void BossState::AttackState::RadialFireAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	//pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	//pos.scale *= 3.5f;
	// 直線
	GenerateBullet(bulletDirect_, bulletSpeed_);

	Vector2 bulletDirect = { bulletDirect_.x,bulletDirect_.z };
	Matrix4x4 yawMatrix = Matrix4x4::MakeRotateYMatrix(boss_->worldTransform_.transform_.rotate.y);
	Vector3 direct = Matrix4x4::TransformVector3(bulletDirect_, yawMatrix);

	float rotValue = LwLib::GetRandomValue(0.1f, 0.5f);
	rotValue = 0.5f;
	float addValue = 1.0f;
	int max = 6;
	for (int i = 0; i < max; ++i) {
		rotValue += addValue;
		// 左回転
		Vector3 newDirect = LwLib::RotateXZVector({ direct.x, 0.0f,direct.z }, -rotValue);
		GenerateBullet(newDirect, bulletSpeed_);
		// 右回転
		newDirect = LwLib::RotateXZVector({ direct.x, 0.0f,direct.z }, rotValue);
		GenerateBullet(newDirect, bulletSpeed_);
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
