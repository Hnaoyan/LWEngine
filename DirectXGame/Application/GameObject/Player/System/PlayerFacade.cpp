#include "PlayerFacade.h"

void PlayerFacade::Initialize(Player* player)
{
	healthManager_.Initialize(player, 20);
	particleManager_.Initialize(player);
	energyManager_.Initialize(player);
}
