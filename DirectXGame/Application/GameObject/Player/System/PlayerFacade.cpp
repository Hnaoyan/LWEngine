#include "PlayerFacade.h"

PlayerFacade::PlayerFacade()
{
	managers_.emplace("Health", std::move(std::make_unique<HealthManager>()));
	managers_.emplace("Particle", std::move(std::make_unique<ParticleManager>()));
	managers_.emplace("Energy", std::move(std::make_unique<EnergyManager>()));
	managers_.emplace("UI", std::move(std::make_unique<PlayerUIManager>()));
	managers_.emplace("Animation", std::move(std::make_unique<AnimationManager>()));
	managers_.emplace("Shooting", std::move(std::make_unique<ShootingManager>()));
	managers_.emplace("JustDodge", std::move(std::make_unique<JustDodgeManager>()));
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
