#pragma once
#include "Engine/Particle/Emitter/ParticleEmitter.h"

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

	class MoveEffect : public ParticleEmitter
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);
	public:
		void SetPlayer(Player* player) { player_ = player; }

	private:
		// 追尾用のポインタ
		Player* player_ = nullptr;

	};


}
