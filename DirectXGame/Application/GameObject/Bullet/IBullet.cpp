#include "IBullet.h"
#include "../../Collision/ColliderFilter.h"
#include "../../GameSystem/GameSystem.h"

uint32_t IBullet::sSerialNumber = 0;

void IBullet::Initialize()
{
	// シリアル番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	InstancedUnit::Initialize();
	transform_.scale *= 0.5f;
	collider_.Initialize(transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeBullet);
	// 生成座標
	generatePosition_ = transform_.translate;
}

void IBullet::Update()
{
	// 生存時間
	lifeTime_ += GameSystem::GameSpeedFactor();
	// 移動
	transform_.translate += velocity_ * GameSystem::GameSpeedFactor();

	InstancedUnit::Update();
	collider_.Update(transform_.translate);
}

void IBullet::ImGuiDraw()
{
}

void IBullet::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<SampleEnemy*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Boss*>(object)) {
		isDead_ = true;
	}
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
}
