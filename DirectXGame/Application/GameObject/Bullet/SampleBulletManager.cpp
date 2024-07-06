#include "SampleBulletManager.h"
#include "Engine/Collision/CollisionManager.h"

void SampleBulletManager::Initialize(Model* model)
{
	model_ = model;
}

void SampleBulletManager::Update()
{
	// 削除処理
	bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(), [](const std::unique_ptr<SampleBullet>& obj) {
		return obj->IsDead();
		}), bullets_.end());
	// 更新
	for (std::vector<std::unique_ptr<SampleBullet>>::iterator it = bullets_.begin();
		it != bullets_.end(); ++it) {
		(*it)->Update();
	}
}

void SampleBulletManager::Draw(ModelDrawDesc desc)
{
	for (std::vector<std::unique_ptr<SampleBullet>>::iterator it = bullets_.begin();
		it != bullets_.end(); ++it) {
		(*it)->Draw(desc);
	}
}

void SampleBulletManager::AddBullet(const GenerateData& data)
{
	std::unique_ptr<SampleBullet> bullet = std::make_unique<SampleBullet>();
	bullet->Initialize(model_);
	bullet->velocity_ = data.velocity;
	bullet->SetPosition(data.position);
	bullet->worldTransform_.isVectorRotate_ = true;
	bullet->worldTransform_.rotateDirect_ = Vector3::Normalize(data.velocity);
	bullets_.push_back(std::move(bullet));
}

void SampleBulletManager::CollisionRegist()
{
	for (std::vector<std::unique_ptr<SampleBullet>>::iterator it = bullets_.begin();
		it != bullets_.end(); ++it) {
		collisionManager_->ListRegist((*it)->GetCollider());
	}
}
