#include "NormalSpinBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"

float NormalSpinBullet::sAcceleration = 0.1f;

void NormalSpinBullet::Initialize()
{
	// 基底クラスの初期化・マスクの設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	GlobalVariables* instance = GlobalVariables::GetInstance();
	sAcceleration = instance->GetValue<float>("BossNormalBullet", "Acceleration");

	moveDirect_ = Vector3::Normalize(velocity_);

}

void NormalSpinBullet::Update()
{
	// 速度の計算
	accelerate_.x += sAcceleration * GameSystem::GameSpeedFactor();
	velocity_ += moveDirect_ * accelerate_.x;
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor() * 3.0f;
	transform_.rotate.y += GameSystem::GameSpeedFactor() * 2.0f;

	// 基底の更新
	IBullet::Update();
}

void NormalSpinBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Player*>(object)) {
		isDead_ = true;
	}
	if (isDead_ && breakEmitter_) {
		static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetPosition(GetWorldPosition());
		static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetEmitFlag(true);
	}
}
