#include "PlayerParticleManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/3D/ModelManager.h"

void PlayerContext::ParticleManager::Initialize(Player* player)
{
	player_ = player;
	// 移動のパーティクル
	std::unique_ptr<ParticleEmitter> instance = std::make_unique<PlayerParticle::MoveEffect>();
	instance->Initialize(ModelManager::GetModel("ParticleCube"));
	PlayerParticle::MoveEffect* pre = static_cast<PlayerParticle::MoveEffect*>(instance.get());
	pre->SetPlayer(player);
	gpuParticle_->CreateEmitter(std::move(instance),"PlayerMove");

}

void PlayerContext::ParticleManager::Update()
{

	if (!std::holds_alternative<IdleState*>(player_->GetVerticalState()->GetNowState())) {

	}


}
