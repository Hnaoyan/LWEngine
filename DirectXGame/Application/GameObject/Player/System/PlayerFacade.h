#pragma once
#include "PlayerSystemLists.h"
#include <unordered_map>
#include <string>
#include <memory>

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
	HealthManager* GetHealth() 
	{ 
		return dynamic_cast<HealthManager*>(managers_.find("Health")->second.get());
	}
	// パーティクル
	ParticleManager* GetParticleManager() {
		return dynamic_cast<ParticleManager*>(managers_.find("Particle")->second.get());
	}
	// エネルギー
	EnergyManager* GetEnergy()
	{
		return dynamic_cast<EnergyManager*>(managers_.find("Energy")->second.get());
	}
	// UI
	PlayerUIManager* GetUI() 
	{
		return dynamic_cast<PlayerUIManager*>(managers_.find("UI")->second.get());
	}

	// Animation
	AnimationManager* GetAnimation()
	{ 
		return dynamic_cast<AnimationManager*>(managers_.find("Animation")->second.get());
	}
	ShootingManager* GetShootingManager() 
	{
		return dynamic_cast<ShootingManager*>(managers_.find("Shooting")->second.get());
	}
	// 回避用
	JustDodgeManager* GetDudgeManager()
	{ 
		return dynamic_cast<JustDodgeManager*>(managers_.find("JustDodge")->second.get());
	}
#pragma endregion

private:
	// システムのリスト
	std::unordered_map<std::string, std::unique_ptr<ISystem>> managers_;
	
};
