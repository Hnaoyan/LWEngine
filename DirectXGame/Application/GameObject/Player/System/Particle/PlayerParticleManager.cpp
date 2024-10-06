#include "PlayerParticleManager.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Application/GameObject/Particle/User/ParticleLists.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/2D/TextureManager.h"

#include <cassert>

void PlayerContext::ParticleManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	// 移動のエミッター作成
	CreateMoveEmitter();
	moveTrail_ = std::make_unique<MoveTrail>();
	moveTrail_->Initialize(player);
}

void PlayerContext::ParticleManager::Update()
{
	// 移動パーティクル
	MoveParticleUpdate();
	moveTrail_->Update();
}

void PlayerContext::ParticleManager::Draw(ICamera* camera)
{
	moveTrail_->Draw(camera);
}

void PlayerContext::ParticleManager::MoveParticleUpdate()
{
	// 生成の制御
	bool moveCheck = std::fabsf(player_->velocity_.x) >= 0.1f || std::fabsf(player_->velocity_.z) >= 0.1f;
	bool isState = std::holds_alternative<IdleVertical*>(player_->VerticalManager()->GetVariant());
	if (moveCheck && isState) {
		//moveParams_.isActive = true;
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

void PlayerContext::MoveTrail::Initialize(Player* player)
{
	assert(player);
	// 
	player_ = player;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.parent_ = &player_->worldTransform_;

	// ポリゴンの初期化
	triangle_ = std::make_unique<Triangle3D>();
	triangle_->Initialize();
	triangle_->SetCamera(player_->camera_);
	triangle_->SetWidth(0.5f);

	// 保存数の最大値
	trailPoint_.second = 20;
}

void PlayerContext::MoveTrail::Update()
{
	// トランスフォームの更新
	worldTransform_.UpdateMatrix();
	// 要素追加
	trailPoint_.first.push_back(worldTransform_.GetWorldPosition());
	// 最大値内に収める
	if (trailPoint_.first.size() > trailPoint_.second) {
		trailPoint_.first.erase(trailPoint_.first.begin());
	}
	// 頂点の更新
	triangle_->Update(trailPoint_.first);
}

void PlayerContext::MoveTrail::Draw(ICamera* camera)
{
	// 描画
	ModelRenderer::TriangleDraw(camera, triangle_.get());
}
