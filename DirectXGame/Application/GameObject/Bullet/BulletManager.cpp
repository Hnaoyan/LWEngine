#include "BulletManager.h"
#include <cassert>

void BulletManager::Initialize(Model* model)
{
	//models_ = models;
	model_ = model;

	trailManager_ = std::make_unique<TrailManager>();

	AddCluster("PlayerNormalBullet");
	AddCluster("PlayerTrackingBullet");
	AddCluster("PlayerDivisionBullet");

	AddCluster("BossTrackingBullet");
	AddCluster("BossNormalBullet");
}

void BulletManager::Update()
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		(*it).second->Update();
	}
}

void BulletManager::Draw(ModelDrawDesc desc)
{
	// 弾の描画
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		(*it).second->Draw(desc);
	}

	// 軌跡の処理
	trailManager_->Draw(desc.camera);
}

void BulletManager::CollisionUpdate(CollisionManager* manager)
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		BulletCluster* obj = static_cast<BulletCluster*>((*it).second.get());
		obj->CollisionUpdate(manager);
	}
}

void BulletManager::AddCluster(const std::string& tag)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<BulletCluster>();
	static_cast<BulletCluster*>(instance.get())->Initialize(model_);
	static_cast<BulletCluster*>(instance.get())->SetTrailManager(trailManager_.get());
	static_cast<BulletCluster*>(instance.get())->SetGPUParticle(gpuParticle_);
	clusters_.emplace(tag, std::move(instance));
}

void BulletManager::AddCluster(const std::string& tag, Model* model)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<BulletCluster>();
	static_cast<BulletCluster*>(instance.get())->Initialize(model);
	static_cast<BulletCluster*>(instance.get())->SetTrailManager(trailManager_.get());
	static_cast<BulletCluster*>(instance.get())->SetGPUParticle(gpuParticle_);
	clusters_.emplace(tag, std::move(instance));
}

void BulletManager::AddCluster(const std::string& tag, Model* model, uint32_t texture)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<BulletCluster>();
	static_cast<BulletCluster*>(instance.get())->Initialize(model);
	static_cast<BulletCluster*>(instance.get())->SetTrailManager(trailManager_.get());
	static_cast<BulletCluster*>(instance.get())->SetGPUParticle(gpuParticle_);
	static_cast<BulletCluster*>(instance.get())->SetTexture(texture);
	clusters_.emplace(tag, std::move(instance));
}

BulletCluster* BulletManager::FindCluster(std::string tag)
{
	// イテレータ取得
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.find(tag);
	// なければエラー
	assert(it != clusters_.end());
	// 見つけたイテレータからポインタを取得
	return static_cast<BulletCluster*>((*it).second.get());
}
