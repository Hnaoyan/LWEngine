#include "TerrainManager.h"
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/LevelEditor/LevelLoader.h"

#include "Terrain.h"
#include "imgui.h"
#include <cassert>

void TerrainManager::Initialize(Model* model)
{
	assert(model);
	model_ = model;

	AddCluster("Terrain");
	AddCluster("Object");

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
	if (isInvisible_) {
		return;
	}
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin();
		it != clusters_.end(); ++it) {
		(*it)->Draw(desc);
	}
}

void TerrainManager::AddCluster()
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<TerrainCluster>();
	static_cast<TerrainCluster*>(instance.get())->Initialize(model_);
	for (std::list<LevelData::ObjectData>::iterator it = LevelLoader::datas_["Terrain"]->objects.begin();
		it != LevelLoader::datas_["Terrain"]->objects.end(); ++it) {
		//if ("Block" != (*it).filename.substr(0, 5)) {
		//	break;
		//}
		//LevelLoader::datas_.find("Block");
		static_cast<TerrainCluster*>(instance.get())->TerrainRegister((*it).transform);
	}
	clusters_.push_back(std::move(instance));
}

void TerrainManager::AddCluster(std::string tag)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<TerrainCluster>();
	if (tag == "Object") {
		static_cast<TerrainCluster*>(instance.get())->name_ = "Object";
		static_cast<TerrainCluster*>(instance.get())->Initialize(ModelManager::GetModel("DefaultCube"));
	}
	else {
		static_cast<TerrainCluster*>(instance.get())->name_ = "Terrain";
		static_cast<TerrainCluster*>(instance.get())->Initialize(ModelManager::GetModel("DefaultCube"));
	}
	
	for (std::list<LevelData::ObjectData>::iterator it = LevelLoader::datas_[tag]->objects.begin();
		it != LevelLoader::datas_[tag]->objects.end(); ++it) {
		static_cast<TerrainCluster*>(instance.get())->TerrainRegister((*it).transform);
	}
	clusters_.push_back(std::move(instance));
}

void TerrainManager::AddCluster(std::string filename,std::string modelTag)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<TerrainCluster>();

	static_cast<TerrainCluster*>(instance.get())->Initialize(ModelManager::GetModel(modelTag));
	LevelLoader::datas_[filename];

	for (std::list<LevelData::ObjectData>::iterator it = LevelLoader::datas_[filename]->objects.begin();
		it != LevelLoader::datas_[filename]->objects.end(); ++it) {
		static_cast<TerrainCluster*>(instance.get())->TerrainRegister((*it).transform);
	}
	clusters_.push_back(std::move(instance));
}

void TerrainManager::ImGuiDraw()
{
	ImGui::Checkbox("IsInvisible", &isInvisible_);
	std::string name = "Cluster";
	if (ImGui::BeginTabBar(name.c_str()))
	{
		for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin();
			it != clusters_.end(); ++it) {
			TerrainCluster* cluster = static_cast<TerrainCluster*>((*it).get());
			name = "Cluster" + std::to_string(cluster->serialNumber_);
			if (ImGui::BeginTabItem(name.c_str()))
			{
				(*it)->ImGuiDraw();
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
}

void TerrainManager::CollisionUpdate(CollisionManager* manager)
{

	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin();
		it != clusters_.end(); ++it) {
		TerrainCluster* obj = static_cast<TerrainCluster*>((*it).get());
		obj->CollisionUpdate(manager);
	}

}
