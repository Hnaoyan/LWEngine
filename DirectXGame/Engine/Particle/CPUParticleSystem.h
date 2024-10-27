#pragma once
#include "Engine/3D/Instancing/InstancedGroup.h"
#include <stdint.h>
#include <vector>
#include <unordered_map>

class CPUParticleSystem
{
public:
	static const uint32_t kNumInstanceMax = 1024;
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>> emitters_;

};

