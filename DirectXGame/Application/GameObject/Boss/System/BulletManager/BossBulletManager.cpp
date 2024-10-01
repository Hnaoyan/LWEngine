#include "BossBulletManager.h"
#include "../BulletCluster/BossBulletCluster.h"

void BossSystemContext::BulletManager::Initialize(Model* model, Boss* boss) {
	model_ = model;
	boss_ = boss;
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
	// セッター処理
	static_cast<BulletCluster*>(instance.get())->SetBossPtr(boss_);
	static_cast<BulletCluster*>(instance.get())->SetGPU(gpuParticle_);
	// 初期化
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

