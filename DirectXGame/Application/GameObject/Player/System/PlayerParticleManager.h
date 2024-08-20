#pragma once
#include <vector>
#include <memory>
#include "../../Particle/User/PlayerParticle.h"
//#include "../Player.h"
class Player;
class GPUParticleSystem;

class PlayerParticleManager
{
public:

	std::vector<std::unique_ptr<PlayerParticle::MoveParticle>> moveEffects_;

public: // アクセッサ
	void SetPlayer(Player* player) {
		player_ = player;
	}
	void SetGPUParticleSystem(GPUParticleSystem* ptr) {
		gpuParticle_ = ptr;
	}

private: // ポインタ関係
	GPUParticleSystem* gpuParticle_ = nullptr;
	Player* player_ = nullptr;
};
