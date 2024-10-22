#pragma once
#include "BulletParticleComponent.h"
#include "Engine/Particle/Emitter/ParticleEmitter.h"

/// <summary>
/// 弾の移動時のパーティクル
/// </summary>
namespace BulletParticle 
{
	class MoveEffect : public ParticleEmitter, public Component
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	};

}
