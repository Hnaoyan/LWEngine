#pragma once
#include "Engine/Particle/Emitter/ParticleEmitter.h"
#include "Application/GameObject/Boss/System/BossSystem.h"

class Boss;

namespace BossParticle
{
	/// <summary>
	/// 弾のエフェクト
	/// </summary>
	class BulletEffect : public ParticleEmitter
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);
	public:
		void SetBullet(InstancedUnit* bullet) { instance_ = bullet; }

	private:
		// エミッターのデータ
		EmitterSphere data_;
		// 
		InstancedUnit* instance_ = nullptr;

	};
	/// <summary>
	/// ダメージを受けたときのエフェクト
	/// </summary>
	class DamageEffect : public ParticleEmitter
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);
	public:
		void SetBoss(Boss* boss) { boss_ = boss; }

	private:
		// エミッターのデータ
		EmitterSphere data_;
		// 
		Boss* boss_ = nullptr;

	};

}
