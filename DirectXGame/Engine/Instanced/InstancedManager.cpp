#include "InstancedManager.h"

void InstancedManager::Update()
{
	// 削除処理
	for (std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin(); it != clusters_.end();) {
		if (it->second->IsDelete()) {
			it = clusters_.erase(it);
		}
		else {
			++it;
		}
	}

	// 更新処理
	for (std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin(); it != clusters_.end();) {
		it->second->Update();
	}
}

void InstancedManager::Draw(ModelDrawDesc desc)
{
	// 描画
	for (std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.begin(); it != clusters_.end();) {
		it->second->Draw(desc);
	}
}

void InstancedManager::CreateCluster(std::unique_ptr<InstancedGroup> instance, const std::string& tag)
{
	// リスト登録
	clusters_.emplace(tag, std::move(instance));
}

InstancedGroup* InstancedManager::FindCluster(const std::string& tag)
{
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.find(tag);
	assert(it != clusters_.end());
	return (*it).second.get();
}

void InstancedManager::DeleteCluster(const std::string& tag)
{
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = clusters_.find(tag);
	assert(it != clusters_.end());
	clusters_.erase(it);
}
