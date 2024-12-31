#include "CPUParticleManager.h"

void CPUParticleManager::Update()
{
	// 削除処理
	emitters_.erase(std::remove_if(emitters_.begin(), emitters_.end(), [](const std::unique_ptr<InstancedGroup>& obj) {
		return obj->IsDelete();
		}), emitters_.end());
	// 更新処理
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = emitters_.begin(); it != emitters_.end(); it++) {
		(*it)->Update();
	}
}

void CPUParticleManager::Draw(ModelDrawDesc desc)
{
	// 描画
	for (std::vector<std::unique_ptr<InstancedGroup>>::iterator it = emitters_.begin(); it != emitters_.end(); it++) {	
		(*it)->Draw(desc);
	}
}
