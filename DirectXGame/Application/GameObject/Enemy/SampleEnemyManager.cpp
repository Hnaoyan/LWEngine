#include "SampleEnemyManager.h"
#include "Engine/Collision/CollisionManager.h"
#include <imgui.h>

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

void SampleEnemyManager::AddEnemy(const GenerateData& data)
{
	std::unique_ptr<SampleEnemy> enemy = std::make_unique<SampleEnemy>();
	enemy->Initialize(model_);
	enemy->worldTransform_.transform_.translate = data.position;
	enemy->Update();
	//enemy->worldTransform_.rotateDirect_ = Vector3::Normalize(data.velocity);
	enemys_.push_back(std::move(enemy));

}

void SampleEnemyManager::CollisionRegist()
{

	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_.begin();
		it != enemys_.end(); ++it) {
		collisionManager_->ListRegist((*it)->GetCollider());
	}

}

void SampleEnemyManager::ImGuiDraw()
{
	ImGui::Begin("Enemys");
	for (std::vector<std::unique_ptr<SampleEnemy>>::iterator it = enemys_.begin();
		it != enemys_.end(); ++it) {
		Vector3 pos = (*it)->worldTransform_.GetWorldPosition();
		std::string name = "Position" + std::to_string((*it)->commonSerialNumber_);
		ImGui::DragFloat3(name.c_str(),&pos.x,0.01f);
		(*it)->worldTransform_.transform_.translate = pos;
	}
	ImGui::End();
}
