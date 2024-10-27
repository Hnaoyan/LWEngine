#pragma once
#include "../BulletParticleComponent.h"
#include "Engine/Particle/Emitter/ParticleEmitter.h"

namespace BulletParticle
{
	class BreakEffect : public Component, public ParticleEmitter
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	public: // セッター
		void SetPosition(const Vector3& position) { emitter_.cMap_->translate = position; }
		void SetEmitFlag(bool flag) { emitter_.cMap_->emit = flag; }
	};

}
