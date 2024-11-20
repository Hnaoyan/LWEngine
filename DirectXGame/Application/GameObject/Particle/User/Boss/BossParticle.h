#pragma once
#include "Engine/Particle/Emitter/GPUParticleEmitter.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include "Application/GameObject/Boss/System/BossSystem.h"
#include "../../EmitterTypes.h"

class Boss;

namespace BossParticle
{
	class EmitterComponent
	{
	public:
		void SetBoss(Boss* boss) { boss_ = boss; }
	protected:
		Boss* boss_ = nullptr;
	};

	/// <summary>
	/// ダメージを受けたときのエフェクト
	/// </summary>
	class DamageEffect : public GPUParticleEmitter, public EmitterComponent
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	};

}
