#include "TerrainManager.h"
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Engine/LevelEditor/LevelLoader.h"

#include "Terrain.h"
#include "imgui.h"
#include <cassert>

void TerrainManager::Initialize(Model* model)
{
	assert(model);
	model_ = model;

	AddCluster();

}

void TerrainManager::Update()
{
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin();
		it != clusters_.end(); ++it) {
		(*it)->Update();
	}
}

void TerrainManager::Draw(ModelDrawDesc desc)
{
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin();
		it != clusters_.end(); ++it) {
		(*it)->Draw(desc);
	}
}

void TerrainManager::AddCluster()
{

	for (std::list<LevelData::ObjectData>::iterator it = LevelLoader::data_->objects.begin();
		it != LevelLoader::data_->objects.end(); ++it) {
		std::unique_ptr<InstancedGroup> instance = std::make_unique<TerrainCluster>();
		static_cast<TerrainCluster*>(instance.get())->Initialize(model_);
		static_cast<TerrainCluster*>(instance.get())->TerrainRegister((*it).transform);
		clusters_.push_back(std::move(instance));
	}
	//std::unique_ptr<InstancedGroup> instance = std::make_unique<TerrainCluster>();
	//static_cast<TerrainCluster*>(instance.get())->Initialize(model_);
	//static_cast<TerrainCluster*>(instance.get())->TerrainRegister({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{10.0f,0.0f,0.0f} });
	//static_cast<TerrainCluster*>(instance.get())->TerrainRegister({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,10.0f,0.0f} });
	//static_cast<TerrainCluster*>(instance.get())->TerrainRegister({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,10.0f} });
	//clusters_.push_back(std::move(instance));
}

void TerrainManager::ImGuiDraw()
{
	ImGui::Begin("TerrainManager");

	ImGui::End();
}

void TerrainManager::CollisionUpdate(CollisionManager* manager)
{

	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin();
		it != clusters_.end(); ++it) {
		TerrainCluster* obj = static_cast<TerrainCluster*>((*it).get());
		obj->CollisionUpdate(manager);
	}

}
