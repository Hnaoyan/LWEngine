#include "PlayerFacade.h"

PlayerFacade::PlayerFacade()
{
	managers_.emplace("Health", std::move(std::make_unique<HealthManager>()));	// HP
	managers_.emplace("Particle", std::move(std::make_unique<ParticleManager>()));	// パーティクル
	managers_.emplace("Energy", std::move(std::make_unique<EnergyManager>()));	// エネルギー
	managers_.emplace("UI", std::move(std::make_unique<PlayerUIManager>()));	// UI
	managers_.emplace("Animation", std::move(std::make_unique<AnimationManager>()));	// アニメーション
	managers_.emplace("Shooting", std::move(std::make_unique<ShootingManager>()));	// 射撃
	managers_.emplace("JustDodge", std::move(std::make_unique<JustDodgeManager>()));	// ジャスト回避
	managers_.emplace("AreaLimiter", std::move(std::make_unique<AreaLimiter>()));	// 座標制限
}

void PlayerFacade::Initialize(Player* player)
{
	for (auto it = managers_.begin(); it != managers_.end(); it++) {
		(*it).second->Initialize(player);
	}
}

void PlayerFacade::Update()
{
	for (auto it = managers_.begin(); it != managers_.end(); it++) {
		(*it).second->Update();
	}
}
