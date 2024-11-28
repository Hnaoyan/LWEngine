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
	energy_.isAssending = std::holds_alternative<AssendingState*>(player_->VerticalState()->GetVariant());
	if (energy_.isAssending) {
		float decrementAssending = 75.0f;
		energy_.currentEnergy -= decrementAssending * GameSystem::GameSpeedFactor();
	}
	else if(!quickBoostRecoveryTime_.IsActive()) {
		if (energy_.maxEnergy > energy_.currentEnergy) {
			float recoveryValue = 5.0f;
			energy_.currentEnergy += recoveryValue * GameSystem::GameSpeedFactor();
		}
	}
}

void PlayerContext::EnergyManager::BoostDecrement()
{
	float defaultDecrement = (energy_.maxEnergy / 5.0f);
	energy_.currentEnergy -= defaultDecrement;
	if (energy_.currentEnergy < 0.0f) {
		energy_.currentEnergy = 0.0f;
	}
	quickBoostRecoveryTime_.Start(10.0f);
}

void PlayerContext::EnergyManager::QuickBoostDecre()
{
	float defaultDecrement = (energy_.maxEnergy / 5.0f);
	float rate = 0.75f;
	energy_.currentEnergy -= (defaultDecrement * rate);
	if (energy_.currentEnergy < 0.0f) {
		energy_.currentEnergy = 0.0f;
	}
	quickBoostRecoveryTime_.Start(10.0f);
}
