#include "PlayerFacade.h"

PlayerFacade::PlayerFacade()
{
	healthManager_ = std::make_unique<HealthManager>();
	particleManager_ = std::make_unique<ParticleManager>();
	energyManager_ = std::make_unique<EnergyManager>();
	uiManager_ = std::make_unique<PlayerUIManager>();
	animationManager_ = std::make_unique<AnimationManager>();
	shootingManager_ = std::make_unique<ShootingManager>();
	justDodgeManager_ = std::make_unique<JustDodgeManager>();
}

void PlayerFacade::Initialize(Player* player)
{
	healthManager_->Initialize(player);
	particleManager_->Initialize(player);
	energyManager_->Initialize(player);
	uiManager_->Initialize(player);
	animationManager_->Initialize(player);
	// 射撃の管理
	shootingManager_->Initialize(player);
	justDodgeManager_->Initialize(player);
}

void PlayerFacade::Update()
{
	healthManager_->Update();
	particleManager_->Update();
	energyManager_->Update();
	animationManager_->Update();
	shootingManager_->Update();
	justDodgeManager_->Update();
}
