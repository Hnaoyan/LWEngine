#include "IPlayerState.h"
#include "../../Player.h"

void IPlayerState::PreInitialize(Player* player)
{
	player_ = player;
	stateManager_ = player->GetStateManager();
	input_ = Input::GetInstance();
}
