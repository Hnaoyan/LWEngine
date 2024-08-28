#include "EnergyManager.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Player/Player.h"

void PlayerContext::EnergyManager::Initialize(Player* player)
{
	player_ = player;
	
	energy_.maxEnergy = 1000.0f;
	energy_.currentEnergy = 1000.0f;

}

void PlayerContext::EnergyManager::Update()
{
	// 上昇中か
	energy_.isAssending = std::holds_alternative<AssendingState*>(player_->GetVerticalState()->GetNowState());
	if (energy_.isAssending) {
		energy_.currentEnergy -= 10.0f * GameSystem::GameSpeedFactor();
	}
	else {
		if (energy_.maxEnergy > energy_.currentEnergy) {
			energy_.currentEnergy += 5.0f * GameSystem::GameSpeedFactor();
		}
	}
}
