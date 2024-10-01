#include "PlayerParticleManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/2D/TextureManager.h"

#include <cassert>

void PlayerContext::ParticleManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	// 移動のエミッター作成
	CreateMoveEmitter();
}

void PlayerContext::ParticleManager::Update()
{
	// 移動パーティクル
	MoveParticleUpdate();

}

void PlayerContext::ParticleManager::MoveParticleUpdate()
{
	// 生成の制御
	bool moveCheck = std::fabsf(player_->velocity_.x) >= 0.1f || std::fabsf(player_->velocity_.z) >= 0.1f;
	bool isState = std::holds_alternative<IdleVertical*>(player_->VerticalManager()->GetVariant());
	if (moveCheck && isState) {
		moveParams_.isActive = true;
	}
	else {
		moveParams_.isActive = false;
	}
}

void PlayerContext::ParticleManager::CreateMoveEmitter()
{
	// 移動のパーティクル
	std::unique_ptr<ParticleEmitter> instance = std::make_unique<PlayerParticle::MoveEffect>();
	instance->Initialize(ModelManager::GetModel("Plane"));
	PlayerParticle::MoveEffect* pre = static_cast<PlayerParticle::MoveEffect*>(instance.get());
	pre->SetPlayer(player_);
	gpuParticle_->CreateEmitter(std::move(instance), "PlayerMove");
}
