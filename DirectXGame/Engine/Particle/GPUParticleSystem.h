#pragma once
#include "Emitter/ParticleEmitter.h"
#include <stdint.h>
#include <vector>

class GPUParticleSystem
{
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 1024;
	// エミッターのリスト
	std::vector<std::unique_ptr<ParticleEmitter>> emitters_;

};