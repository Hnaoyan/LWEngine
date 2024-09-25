#pragma once
#include <vector>
#include <memory>
#include "Application/GameObject/Particle/User/ParticleLists.h"

class Player;
class GPUParticleSystem;

namespace PlayerContext {
	// 移動のパラメータ
	struct MoveParams 
	{
		bool isActive = false;
		uint32_t texture = 0u;
	};

	/// <summary>
	/// マネージャ
	/// </summary>
	class ParticleManager
	{
	public:
		void Initialize(Player* player);
		void Update();

	private: // 移動
		void MoveParticleUpdate();
		void CreateMoveEmitter();
		MoveParams moveParams_;

	private: // ポインタ関係
		GPUParticleSystem* gpuParticle_ = nullptr;
		Player* player_ = nullptr;

	public: // アクセッサ
		void SetGPUParticleSystem(GPUParticleSystem* ptr) { gpuParticle_ = ptr; }
		bool IsMove() { return moveParams_.isActive; }
	};
}
