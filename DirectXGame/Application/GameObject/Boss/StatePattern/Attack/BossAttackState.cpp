#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::AttackState::Initialize()
{
	changeTimer_.Start(90.0f);
	fireTimer_.Start(10.0f);

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
		LockAttack();
		fireTimer_.Start(10.0f);
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
