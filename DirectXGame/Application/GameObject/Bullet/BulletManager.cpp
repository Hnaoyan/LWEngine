#include "BulletManager.h"
#include <cassert>

void BulletManager::Initialize(Model* model)
{
	//models_ = models;
	model_ = model;

	AddCluster("NormalBullet");
	AddCluster("HighBullet");
}

void BulletManager::Update()
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		BulletCluster* obj = static_cast<BulletCluster*>((*it).second.get());
		obj->Update();
	}
}

void BulletManager::Draw(ModelDrawDesc desc)
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		BulletCluster* obj = static_cast<BulletCluster*>((*it).second.get());
		obj->Draw(desc);
	}
}

void BulletManager::CollisionUpdate(CollisionManager* manager)
{
	for (auto it = clusters_.begin(); it != clusters_.end(); ++it) {
		BulletCluster* obj = static_cast<BulletCluster*>((*it).second.get());
		obj->CollisionUpdate(manager);
	}
}

void BulletManager::AddCluster(std::string tag)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<BulletCluster>();
	static_cast<BulletCluster*>(instance.get())->Initialize(model_);
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
