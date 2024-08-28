#include "EnergyManager.h"
#include "Application/GameObject/Player/Player.h"

void PlayerContext::EnergyManager::Initialize(Player* player)
{
	player_ = player;
}

void PlayerContext::EnergyManager::Update()
{
	// 上昇中か
	energy_.isAssending = std::holds_alternative<AssendingState*>(player_->GetVerticalState()->GetNowState());
	if (energy_.isAssending) {

	}


}
