#pragma once
#include "Engine/Particle/Emitter/ParticleEmitter.h"

namespace PlayerParticle
{
	class MoveParticle : public ParticleEmitter
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);
				
	private:
		// エミッターのデータ
		EmitterSphere data_;
	};


}
