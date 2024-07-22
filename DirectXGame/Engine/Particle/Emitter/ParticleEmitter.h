#pragma once
#include "../ParticleCBuffer.h"

class ParticleEmitter
{
public:
	//void Initialize();
	//void Update();

	void CreateData();

	//HeapAllocationData 
	// GPU関係データ
	ConstantBufferMapContext<EmitterSphere> emit_;
	ConstantBufferMapContext<PerFrame> perFrame_;

};