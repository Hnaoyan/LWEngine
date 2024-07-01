#include "SampleBulletManager.h"

void SampleBulletManager::Initialize(Model* model)
{
	model_ = model;
}

void SampleBulletManager::Update()
{
	for (std::vector<std::unique_ptr<SampleBullet>>::iterator it = bullets_.begin();
		it != bullets_.end(); ++it) {
		(*it)->Update();
	}
	bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(), [](const std::unique_ptr<SampleBullet>& obj) {
		return obj->isDead_;
		}), bullets_.end());
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
	bullets_.push_back(std::move(bullet));
}
