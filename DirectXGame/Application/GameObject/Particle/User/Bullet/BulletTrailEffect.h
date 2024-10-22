#pragma once
#include "BulletParticleComponent.h"
#include "Engine/Particle/Emitter/ParticleEmitter.h"

namespace BulletParticle {
	class TrailEffect : public ParticleEmitter, public Component
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	};
}
