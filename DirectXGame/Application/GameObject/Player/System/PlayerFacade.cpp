#include "PlayerFacade.h"

void PlayerFacade::Initialize(Player* player)
{
	healthManager_.Initialize(player, 20);
	particleManager_.Initialize(player);
	energyManager_.Initialize(player);
	uiManager_.Initialize(player);
	animationManager_.Initialize(player);
	// 射撃の管理
	shootingManager_.Initialize(player);
}

void PlayerFacade::Update()
{

	healthManager_.Update();
	particleManager_.Update();
	energyManager_.Update();
	uiManager_;
	animationManager_.Update();

	shootingManager_.Update();
}
