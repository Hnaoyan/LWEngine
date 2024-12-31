#pragma once
#include "CPU/Emitter/CPUParticleEmitter.h"
#include <vector>
#include <memory>

class CPUParticleManager
{
public:
	void Update();
	void Draw(ModelDrawDesc desc);

private:
	std::vector<std::unique_ptr<InstancedGroup>> emitters_;

};
