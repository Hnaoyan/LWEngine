#pragma once
#include "PlayerSystemLists.h"
#include <unordered_map>

class Player;
namespace PlayerContext {};

using namespace PlayerContext;

/// <summary>
/// プレイヤーのシステム管理
/// </summary>
class PlayerFacade
{
public:
	PlayerFacade();
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
	HealthManager* GetHealth() { return healthManager_.get(); }
	// パーティクル
	ParticleManager* GetParticleManager() { return particleManager_.get(); }
	// エネルギー
	EnergyManager* GetEnergy() { return energyManager_.get(); }
	// UI
	PlayerUIManager* GetUI() { return uiManager_.get(); }
	// Animation
	AnimationManager* GetAnimation() { return animationManager_.get(); }
	ShootingManager* GetShootingManager() { return shootingManager_.get(); }
	// 回避用
	JustDodgeManager* GetDudgeManager() { return justDodgeManager_.get(); }
#pragma endregion

private:
	// Hp関係
	std::unique_ptr<HealthManager> healthManager_;
	// パーティクル
	std::unique_ptr<ParticleManager> particleManager_;
	// エネルギー
	std::unique_ptr<EnergyManager> energyManager_;
	// UI
	std::unique_ptr<PlayerUIManager> uiManager_;
	// Animation
	std::unique_ptr<AnimationManager> animationManager_;
	// 射撃関係のマネージャー
	std::unique_ptr<ShootingManager> shootingManager_;
	// ジャスト回避などの無敵処理関係
	std::unique_ptr<JustDodgeManager> justDodgeManager_;
	//// Aim
	//PlayerContext::AimManager aimManager_;
	//// LockOn
	//PlayerContext::LockOn lockOn_;


	
};
