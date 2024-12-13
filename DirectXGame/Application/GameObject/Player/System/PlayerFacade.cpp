#include "PlayerFacade.h"

void PlayerFacade::Initialize(Player* player)
{
	const int32_t maxHP = 20;
	healthManager_.Initialize(player, maxHP);
	particleManager_.Initialize(player);
	energyManager_.Initialize(player);
	uiManager_.Initialize(player);
	animationManager_.Initialize(player);
	// 射撃の管理
	shootingManager_.Initialize(player);

	justDodgeManager_ = PlayerContext::JustDodgeManager(player);
}

void PlayerFacade::Update()
{
	healthManager_.Update();
	particleManager_.Update();
	energyManager_.Update();
	animationManager_.Update();
	shootingManager_.Update();
	justDodgeManager_.Update();
}
