#include "../BossBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"

float BossSystemContext::NormalBullet::sAcceleration = 0.1f;

void BossSystemContext::NormalBullet::Initialize()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	sAcceleration = instance->GetValue<float>("BossNormalBullet", "Acceleration");
	acceleration_ = 0.0f;

	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	tag_ = "BossBullet" + std::to_string(serialNumber_);
	
	InstancedUnit::Initialize();
	transform_ = { {1.0f,1.0f,1.0f} ,{0,0,0},{0.0f,0.0f,0.0f} };
	collider_.Initialize(transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	moveDirect_ = Vector3::Normalize(velocity_);

	// 軌跡
	trail_ = std::make_unique<BulletTrail>();
	trail_->triangle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });

}

void BossSystemContext::NormalBullet::Update()
{
	// 速度の計算
	acceleration_ += sAcceleration * GameSystem::GameSpeedFactor();
	velocity_ += moveDirect_ * acceleration_;
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor() * 3.0f;
	transform_.rotate.y += GameSystem::GameSpeedFactor() * 2.0f;

	// 基底の更新
	IBullet::Update();
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
