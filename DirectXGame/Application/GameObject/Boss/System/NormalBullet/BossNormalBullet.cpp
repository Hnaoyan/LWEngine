#include "../BossSystem.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"

void BossSystemContext::NormalBullet::Initialize()
{
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	tag_ = "BossBullet" + std::to_string(serialNumber_);
	
	InstancedUnit::Initialize();
	transform_ = { {1.0f,1.0f,1.0f} ,{0,0,0},{0.0f,0.0f,0.0f} };
	transform_.scale *= 0.3f;
	collider_.Initialize(transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

}

void BossSystemContext::NormalBullet::Update()
{
	// 移動
	transform_.translate += velocity_ * GameSystem::GameSpeedFactor();

	InstancedUnit::Update();
	collider_.Update(transform_.translate);
}

void BossSystemContext::NormalBullet::ImGuiDraw()
{

}

void BossSystemContext::NormalBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Player*>(object)) {
		isDead_ = true;
	}
}
