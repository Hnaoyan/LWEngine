#include "ModelManager.h"

std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::sModels_;

void ModelManager::LoadAnimModel(const std::string& tag, const std::string& filePath)
{
	std::unordered_map<std::string, std::unique_ptr<Model>>::iterator it = sModels_.find(tag);
	if (it != sModels_.end()) {
		return;
	}
	else {
		std::unique_ptr<Model> model;
		model.reset(Model::CreateObj(filePath, LoadExtension::kGltf));
		sModels_.emplace(tag, std::move(model));
	}
	return;
}

void ModelManager::LoadNormalModel(const std::string& tag, const std::string& filePath)
{
	std::unordered_map<std::string, std::unique_ptr<Model>>::iterator it = sModels_.find(tag);
	if (it != sModels_.end()) {
		return;
	}
	else {
		std::unique_ptr<Model> model;
		model.reset(Model::CreateDefault(filePath));
		sModels_.emplace(tag, std::move(model));
	}
	return;
}

Model* ModelManager::GetModel(const std::string& tag)
{
	// イテレータ取得
	std::unordered_map<std::string, std::unique_ptr<Model>>::iterator it = sModels_.find(tag);
	// なければエラー
	assert(it != sModels_.end());
	// 見つけたイテレータからポインタを取得
	return it->second.get();
}
