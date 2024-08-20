#include "PlayerParticleManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/Particle/GPUParticleSystem.h"

void PlayerParticleManager::Initialize(Player* player)
{
	player_ = player;


}

void PlayerParticleManager::Update()
{

	if (!std::holds_alternative<IdleState*>(player_->GetVerticalState()->GetNowState())) {

	}


}
