#include "CPUParticleManager.h"

CPUParticleManager::CPUParticleManager()
{

}

void CPUParticleManager::Update()
{
	// 基底
	InstancedManager::Update();
}

void CPUParticleManager::Draw(ModelDrawDesc desc)
{
	// 基底
	InstancedManager::Draw(desc);
}
