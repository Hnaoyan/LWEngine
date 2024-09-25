#pragma once
#include <vector>
#include <memory>
#include "Application/GameObject/Particle/User/ParticleLists.h"

class Player;
class GPUParticleSystem;

namespace PlayerContext {
	class ParticleManager
	{
	public:
		void Initialize(Player* player);
		void Update();

	public: // アクセッサ
		void SetGPUParticleSystem(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }

	private:
		void CreateMoveEmitter();

	private: // ポインタ関係
		GPUParticleSystem* gpuParticle_ = nullptr;
		Player* player_ = nullptr;
	};
}
