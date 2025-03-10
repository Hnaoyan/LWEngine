#include "CPUParticleManager.h"

CPUParticleManager::CPUParticleManager()
{
	emitters_.clear();
}

void CPUParticleManager::Update()
{
	// 削除処理
	for (std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = emitters_.begin(); it != emitters_.end();) {
		if (it->second->IsDelete()) {
			it = emitters_.erase(it);
		}
		else {
			++it;
		}
	}

	// 更新処理
	for (std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = emitters_.begin(); it != emitters_.end();) {
		it->second->Update();
	}
}

void CPUParticleManager::Draw(ModelDrawDesc desc)
{
	// 描画
	for (std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = emitters_.begin(); it != emitters_.end();) {
		it->second->Draw(desc);
	}
}

void CPUParticleManager::CreateEmitter(const std::string& tag)
{
	std::unique_ptr<InstancedGroup> instance = std::make_unique<InstancedGroup>();
	emitters_.emplace(tag, std::move(instance));
}

void CPUParticleManager::DeleteEmitter(const std::string& tag)
{
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = emitters_.find(tag);
	assert(it != emitters_.end());
	emitters_.erase(it);
}

InstancedGroup* CPUParticleManager::FindEmitter(const std::string& tag)
{
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>>::iterator it = emitters_.find(tag);
	assert(it != emitters_.end());
	return (*it).second.get();
}
