#pragma once
#include "Engine/Particle/Emitter/ParticleEmitter.h"

class Player;

namespace PlayerParticle
{
	class MoveEffect : public ParticleEmitter
	{
	public:
		virtual void Initialize(Model* model, uint32_t textureHandle = 0);
		virtual void Update();
		virtual void Draw(ICamera* camera);
	public:
		void SetPlayer(Player* player) {
			player_ = player;
		}

	private:
		// エミッターのデータ
		EmitterSphere data_;
		// 追尾用のポインタ
		Player* player_ = nullptr;

	};


}
