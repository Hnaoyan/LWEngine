#include "BulletManager.h"
#include <cassert>

void BulletManager::Initialize(Model* model)
{
	//models_ = models;
	model_ = model;
	AddCluster();
	AddCluster();
}

void BulletManager::Update()
{
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
		it != bulletClusters_.end(); ++it) {
		(*it)->Update();
	}
}

void BulletManager::Draw(ModelDrawDesc desc)
{
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
		it != bulletClusters_.end(); ++it) {
		(*it)->Draw(desc);
	}
}

void BulletManager::CollisionUpdate(CollisionManager* manager)
{
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = bulletClusters_.begin();
		it != bulletClusters_.end(); ++it) {
		BulletCluster* obj = static_cast<BulletCluster*>((*it).get());
		obj->CollisionUpdate(manager);
	}

}

void BulletManager::AddCluster()
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<BulletCluster>();
	static_cast<BulletCluster*>(instance.get())->Initialize(model_);
	bulletClusters_.push_back(std::move(instance));
}
