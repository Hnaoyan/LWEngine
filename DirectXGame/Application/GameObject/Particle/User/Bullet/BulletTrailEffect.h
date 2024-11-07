#pragma once
#include "BulletParticleComponent.h"
#include "Engine/Particle/Emitter/GPUParticleEmitter.h"

namespace BulletParticle {
	class TrailEffect : public GPUParticleEmitter, public Component
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	};
}
