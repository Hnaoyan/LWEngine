#pragma once
#include "PlayerSystemLists.h"

class Player;

/// <summary>
/// プレイヤーのシステム管理
/// </summary>
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
	PlayerContext::PlayerUIManager* GetUI() { return &uiManager_; }
	// Animation
	PlayerContext::AnimationManager* GetAnimation() { return &animationManager_; }
	PlayerContext::ShootingManager* GetShootingManager() { return &shootingManager_; }

	// 回避用
	PlayerContext::JustDodgeManager* GetDudgeManager() { return &justDodgeManager_; }
#pragma endregion

private:
	// Hp関係
	PlayerContext::HealthManager healthManager_;
	// パーティクル
	PlayerContext::ParticleManager particleManager_;
	// エネルギー
	PlayerContext::EnergyManager energyManager_;
	// UI
	PlayerContext::PlayerUIManager uiManager_;
	// Animation
	PlayerContext::AnimationManager animationManager_;
	// 射撃関係のマネージャー
	PlayerContext::ShootingManager shootingManager_;
	// ジャスト回避などの無敵処理関係
	PlayerContext::JustDodgeManager justDodgeManager_;
	//// Aim
	//PlayerContext::AimManager aimManager_;
	//// LockOn
	//PlayerContext::LockOn lockOn_;
};
