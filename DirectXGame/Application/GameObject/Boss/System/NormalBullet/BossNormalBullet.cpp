#include "../BossSystem.h"
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
	//transform_.scale *= 0.5f;
	collider_.Initialize(transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	moveDirect_ = Vector3::Normalize(velocity_);
}

void BossSystemContext::NormalBullet::Update()
{
	//velocity_.x += moveDirect_.x * 2.0f;
	//velocity_.z += moveDirect_.z * 2.0f;
	
	acceleration_ += sAcceleration * GameSystem::GameSpeedFactor();
	velocity_ += moveDirect_ * acceleration_;
	// 移動
	transform_.translate += velocity_ * GameSystem::GameSpeedFactor();
	transform_.rotate.x += GameSystem::GameSpeedFactor() * 3.0f;
	transform_.rotate.y += GameSystem::GameSpeedFactor() * 2.0f;

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
