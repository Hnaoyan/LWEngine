#pragma once
#include "PlayerSystemLists.h"

class Player;

class PlayerFacade
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ
#pragma region アクセッサ
	// Hp関係
	PlayerContext::HealthManager* GetHealth() { return &healthManager_; }
	// パーティクル
	PlayerContext::ParticleManager* GetParticleManager() { return &particleManager_; }
	// エネルギー
	PlayerContext::EnergyManager* GetEnergy() { return &energyManager_; }
	// UI
	PlayerContext::UIManager* GetUI() { return &uiManager_; }
	// Animation
	PlayerContext::AnimationManager* GetAnimation() { return &animationManager_; }
	//// Aim
	//PlayerContext::AimManager* GetAim() { return &aimManager_; }
	//// LockOn
	//PlayerContext::LockOn* GetLockOn() { return &lockOn_; }
#pragma endregion

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
	//// Aim
	//PlayerContext::AimManager aimManager_;
	//// LockOn
	//PlayerContext::LockOn lockOn_;
};
