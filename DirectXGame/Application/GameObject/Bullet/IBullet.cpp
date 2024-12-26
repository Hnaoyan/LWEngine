#include "IBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Particle/User/ParticlePaths.h"
#include "Application/GameObject/IGameObject.h"

#include <imgui.h>

uint32_t IBullet::sSerialNumber = 0;

void IBullet::Initialize()
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;
	// タグ設定
	tag_ = "Bullet" + std::to_string(serialNumber_);

	InstancedUnit::Initialize();
	collider_.Initialize(transform_.scale.x, this);
	// 生成座標
	float defaultRatio = 1.0f;
	damageRatio_ = defaultRatio;
}

void IBullet::Update()
{
	// 移動
	transform_.translate += velocity_ * GameSystem::GameSpeedFactor();
	// サイズの設定
	float scaleRatio = 2.0f;
	collider_.radius_ = transform_.scale.x * scaleRatio;
	collider_.Update(transform_.translate);
	// ユニットの基底
	InstancedUnit::Update();
}

void IBullet::ImGuiDraw()
{
	
}

void IBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<Boss*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	//if (isDead_ && breakEmitter_) {
	//	static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetPosition(GetWorldPosition());
	//	static_cast<BulletParticle::BreakEffect*>(breakEmitter_)->SetEmitFlag(true);
	//}
}

float IBullet::TargetToDistance()
{
	// ターゲットいなかったら
	assert(targetObject_);
	// 距離概算
	Vector3 bullet = GetWorldPosition();
	Vector3 target = targetObject_->worldTransform_.GetWorldPosition();
	return Vector3::Distance(bullet, target);
}
