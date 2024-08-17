#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"

void BossState::MissileAttackState::Initialize()
{
	//---弾の情報---//
	// 速さ
	bulletSpeed_ = 50.0f;
	// サイズ
	bulletScale_ = 0.4f;
	// 進む方向
	bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
	MissileAttack();
	changeTimer_.Start(120.0f);
}

void BossState::MissileAttackState::Update()
{

	TimerUpdate(this);
}

void BossState::MissileAttackState::Exit()
{

}

void BossState::MissileAttackState::MissileAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, {0.0f,1.0f,0.0f}, bulletSpeed_, boss_->GetPlayer());
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, { 1.0f,0.0f,0.0f }, bulletSpeed_, boss_->GetPlayer());
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, { -1.0f,0.0f,0.0f }, bulletSpeed_, boss_->GetPlayer());
}
