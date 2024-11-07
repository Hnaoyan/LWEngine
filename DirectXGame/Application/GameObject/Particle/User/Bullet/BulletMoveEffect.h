#pragma once
#include "BulletParticleComponent.h"
#include "Engine/Particle/Emitter/GPUParticleEmitter.h"

/// <summary>
/// 弾の移動時のパーティクル
/// </summary>
namespace BulletParticle 
{
	class MoveEffect : public GPUParticleEmitter, public Component
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	public:
		void SetEmitPattern(uint32_t pattern) {
			data_.emitPattern = pattern;
			RefreshData(data_);
		}

	};

}
