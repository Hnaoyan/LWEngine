#pragma once
#include "BossParticle.h"

namespace BossParticle
{
	
	class BarrierBreakEffect : public GPUParticleEmitter, public EmitterComponent
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	};

}
