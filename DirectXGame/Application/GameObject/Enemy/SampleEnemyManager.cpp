#include "SampleEnemyManager.h"
#include "Engine/Collision/CollisionManager.h"

void SampleEnemyManager::Initialize(Model* model)
{
	model_ = model;
}

void SampleEnemyManager::Update()
{
	// 削除処理
	enemys_.erase(std::remove_if(enemys_.begin(), enemys_.end(), [](const std::unique_ptr<SampleEnemy>& obj) {
		return obj->IsDead();
		}), enemys_.end());
	// 更新
	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_.begin();
		it != enemys_.end(); ++it) {
		(*it)->Update();
	}

}

void SampleEnemyManager::Draw(ModelDrawDesc desc)
{
	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_.begin();
		it != enemys_.end(); ++it) {
		(*it)->Draw(desc);
	}
}

void SampleEnemyManager::CollisionRegist()
{

	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_.begin();
		it != enemys_.end(); ++it) {
		collisionManager_->ListRegist((*it)->GetCollider());
	}

}
