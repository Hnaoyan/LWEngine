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
		virtual void UpdateEmitter(const EmitterSphere& data);
		
	private:
		// エミッターのデータ
		EmitterSphere data_;
		// パーティクルのデータ
		ParticleCS particleData_;
	};


}
