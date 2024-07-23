#pragma once
#include "../ParticleCBuffer.h"

class ParticleEmitter
{
public:
	//void Initialize();
	//void Update();

	void CreateData();

	RWStructuredBufferContext<ParticleCS> particles_;
};