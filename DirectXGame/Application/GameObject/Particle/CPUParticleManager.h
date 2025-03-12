#pragma once
#include "CPU/Emitter/CPUParticleEmitter.h"
#include "Engine/Instanced/InstancedManager.h"
#include <vector>
#include <memory>
#include <unordered_map>

/// <summary>
/// CPUのパーティクル管理
/// </summary>
class CPUParticleManager : public InstancedManager
{
public:
	CPUParticleManager();
	void Update() override;
	void Draw(ModelDrawDesc desc) override;

};
