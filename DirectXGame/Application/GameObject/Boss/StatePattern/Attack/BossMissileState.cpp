#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"

void BossState::MissileAttackState::Initialize()
{
	boss_->SetNowVariantState(this);
	RotateUpdate();
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
	boss_->SetPrevVariantState(this);
}

void BossState::MissileAttackState::MissileAttack()
{
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 上
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, {0.0f,1.0f,0.0f}, bulletSpeed_, boss_->GetPlayer());
	
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(boss_->worldTransform_.transform_.rotate.y);
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);
	Vector3 rotateVector = Matrix4x4::TransformVector3({ 1.0f,1.0f,0.0f }, rotateMatrix);
	//---斜め---//
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -1.0f,1.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	// X強め
	rotateVector = Matrix4x4::TransformVector3({ 2.0f,1.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -2.0f,1.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());

	rotateVector = Matrix4x4::TransformVector3({ 3.0f,1.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -3.0f,1.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());

	// Y強め
	rotateVector = Matrix4x4::TransformVector3({ 1.0f,2.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -1.0f,2.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());

	rotateVector = Matrix4x4::TransformVector3({ 1.0f,3.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -1.0f,3.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());

	rotateVector = Matrix4x4::TransformVector3({ 1.0f,4.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -1.0f,4.0f,0.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());

	rotateVector = Matrix4x4::TransformVector3({ 1.0f,5.0f,2.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	rotateVector = Matrix4x4::TransformVector3({ -1.0f,5.0f,2.0f }, rotateMatrix);
	boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, rotateVector, bulletSpeed_, boss_->GetPlayer());
	// 左右
	//boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, { 1.0f,0.0f,0.0f }, bulletSpeed_, boss_->GetPlayer());
	//boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, { -1.0f,0.0f,0.0f }, bulletSpeed_, boss_->GetPlayer());
}
