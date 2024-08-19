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
		manager->ListRegist(static_cast<IBullet*>((*it).get())->GetCollider());
	}
}

void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct) {
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
	// 速度
	static_cast<NormalBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * 5.0f);
	static_cast<NormalBullet*>(instance.get())->Initialize();
	instance->transform_ = transform;
	instance->Update();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
void BossSystemContext::BulletCluster::AddBullet(const EulerTransform& transform, const Vector3& direct,float speed) {
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<NormalBullet>();
	// 速度
	static_cast<NormalBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
	static_cast<NormalBullet*>(instance.get())->Initialize();
	instance->transform_ = transform;
	instance->Update();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
void BossSystemContext::BulletCluster::AddMissile(const EulerTransform& transform, const Vector3& direct, float speed, Player* player)
{
	// インスタンス作成
	std::unique_ptr<InstancedUnit> instance = std::make_unique<TrackingBullet>();
	// 速度
	static_cast<TrackingBullet*>(instance.get())->SetVelocity(Vector3::Normalize(direct) * speed);
	static_cast<TrackingBullet*>(instance.get())->SetPlayer(player);
	static_cast<TrackingBullet*>(instance.get())->Initialize();
	instance->transform_ = transform;
	instance->Update();
	// リストにムーブ
	units_.push_back(std::move(instance));
}
#pragma endregion

void BossSystemContext::BulletManager::Initialize(Model* model) {
	model_ = model;
	// 弾用
	AddCluster();
	// ミサイル用
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
		BulletCluster* obj = static_cast<BulletCluster*>((*it).get());
		obj->CollisionUpdate(manager);
	}
}

void BossSystemContext::BulletManager::AddCluster() {
	std::unique_ptr<InstancedGroup> instance = std::make_unique<BulletCluster>();
	static_cast<BulletCluster*>(instance.get())->Initialize(model_);
	bulletClusters_.push_back(std::move(instance));
}

BossSystemContext::BulletCluster* BossSystemContext::BulletManager::GetBeginCluster() {
	return static_cast<BulletCluster*>((*bulletClusters_.begin()).get());
}

BossSystemContext::BulletCluster* BossSystemContext::BulletManager::GetMissileCluster()
{
	std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
	it++;
	return static_cast<BulletCluster*>((*it).get());
}
