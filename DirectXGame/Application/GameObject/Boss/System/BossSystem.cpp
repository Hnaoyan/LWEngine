#include "BossSystem.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/2D/TextureManager.h"
#include "../../../Collision/ColliderFilter.h"
#include "../../../GameSystem/GameSystem.h"

void BossSystemContext::HealthManager::TakeDamage(int32_t damage)
{
	currentHealth_ -= damage;
	// 死亡処理
	if (currentHealth_ < 0) {
		isDead_ = true;
		currentHealth_ = 0;
	}
}

void BossSystemContext::HealthManager::Heal(int32_t heal)
{
	if (currentHealth_ < maxHealth_) {
		currentHealth_ += heal;
	}
}
#pragma region Cluster
void BossSystemContext::BulletCluster::Initialize(Model* model) {
	// 基底クラス初期化
	InstancedGroup::Initialize(model);
	texture_ = TextureManager::GetInstance()->Load("Resources/default/red2x2.png");
}

void BossSystemContext::BulletCluster::Update() {
	// 死亡処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [](const std::unique_ptr<InstancedUnit>& obj) {
		return obj->IsDead();
		}), units_.end());
	// 基底クラス更新
	InstancedGroup::Update();
}

void BossSystemContext::BulletCluster::Draw(ModelDrawDesc desc) {
	// 描画
	model_->InstancedDraw(desc, this->unitNum_, srvHandles_.second, texture_);
}

void BossSystemContext::BulletCluster::ImGuiDraw() {

}

void BossSystemContext::BulletCluster::CollisionUpdate(CollisionManager* manager) {
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {
		manager->ListRegist(static_cast<NormalBullet*>((*it).get())->GetCollider());
	}
}

void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct) {
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
	instance->transform_ = transform;
	// 速度
	static_cast<NormalBullet*>(instance.get())->SetVelocity(direct * 5.0f);
	static_cast<NormalBullet*>(instance.get())->Initialize();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct,float speed) {
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
	instance->transform_ = transform;
	// 速度
	static_cast<NormalBullet*>(instance.get())->SetVelocity(direct * speed);
	static_cast<NormalBullet*>(instance.get())->Initialize();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
#pragma endregion

void BossSystemContext::NormalBullet::Initialize()
{
	InstancedUnit::Initialize();
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

void BossSystemContext::BulletManager::Initialize(Model* model) {
	model_ = model;
	AddCluster();

}

void BossSystemContext::BulletManager::Update() {
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
		it != bulletClusters_.end(); ++it) {
		(*it)->Update();
	}
}

void BossSystemContext::BulletManager::Draw(ModelDrawDesc desc) {
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
		it != bulletClusters_.end(); ++it) {
		(*it)->Draw(desc);
	}
}

void BossSystemContext::BulletManager::CollisionUpdate(CollisionManager* manager) {
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
		it != bulletClusters_.end(); ++it) {
		Cluster* obj = static_cast<Cluster*>((*it).get());
		obj->CollisionUpdate(manager);
	}
}

void BossSystemContext::BulletManager::AddCluster() {
	std::unique_ptr<InstancedGroup> instance = std::make_unique<Cluster>();
	static_cast<Cluster*>(instance.get())->Initialize(model_);
	bulletClusters_.push_back(std::move(instance));
}

