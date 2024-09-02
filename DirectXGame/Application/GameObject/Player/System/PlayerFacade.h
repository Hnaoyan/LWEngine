#pragma once
#include "PlayerSystemLists.h"

class Player;

class PlayerFacade
{
public:
	void Initialize(Player* player);
	void Update();

public: // アクセッサ
	// Hp関係
	PlayerContext::HealthManager* GetHealth() { return &healthManager_; }
	// パーティクル
	PlayerContext::ParticleManager* GetParticle() { return &particleManager_; }
	// エネルギー
	PlayerContext::EnergyManager* GetEnergy() { return &energyManager_; }
	// UI
	PlayerContext::UIManager* GetUI() { return &uiManager_; }
	// Animation
	PlayerContext::AnimationManager* GetAnimation() { return &animationManager_; }

private:
	// Hp関係
	PlayerContext::HealthManager healthManager_;
	// パーティクル
	PlayerContext::ParticleManager particleManager_;
	// エネルギー
	PlayerContext::EnergyManager energyManager_;
	// UI
	PlayerContext::UIManager uiManager_;
	// Animation
	PlayerContext::AnimationManager animationManager_;

};
