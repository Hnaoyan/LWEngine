#pragma once
#include "Engine/Particle/Emitter/ParticleEmitter.h"
#include "Engine/3D/Instancing/InstancedUnit.h"

class Player;

namespace PlayerParticle
{
	class EmitterComponent
	{
	public:
		void SetPlayer(Player* player) { player_ = player; }
	protected:
		Player* player_ = nullptr;
	};

	class MoveEffect : public ParticleEmitter, public EmitterComponent
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	private:

	};

	class BulletEffect : public ParticleEmitter, public EmitterComponent
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);

	private:
		// 
		InstancedUnit* instance_ = nullptr;
	};

}
