#include "EnergyManager.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Player/Player.h"

void PlayerContext::EnergyManager::Initialize(Player* player)
{
	player_ = player;
	
	energy_.maxEnergy = 100.0f;
	energy_.currentEnergy = energy_.maxEnergy;

}

void PlayerContext::EnergyManager::Update()
{
	// 上昇中か
	quickBoostRecoveryTime_.Update();
	energy_.isAssending = std::holds_alternative<AssendingState*>(player_->VerticalManager()->GetVariant());
	if (energy_.isAssending) {
		energy_.currentEnergy -= 75.0f * GameSystem::GameSpeedFactor();
	}
	//else /*if(std::holds_alternative<IdleVertical*>(player_->GetVerticalState()->GetNowState()))*/{
	//	if (energy_.maxEnergy > energy_.currentEnergy) {
	//		energy_.currentEnergy += 5.0f * GameSystem::GameSpeedFactor();
	//	}
	//}
	else if(!quickBoostRecoveryTime_.IsActive()) {
		if (energy_.maxEnergy > energy_.currentEnergy) {
			energy_.currentEnergy += 5.0f * GameSystem::GameSpeedFactor();
		}
	}
}

void PlayerContext::EnergyManager::QuickBoostDecre()
{
	energy_.currentEnergy -= (energy_.maxEnergy / 5.0f);
	quickBoostRecoveryTime_.Start(10.0f);
}
