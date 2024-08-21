#include "../User/PlayerParticle.h"
#include "Application/GameObject/GameObjectLists.h"

void PlayerParticle::MoveParticle::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);

	RefreshData(data_);

}

void PlayerParticle::MoveParticle::Update()
{
	// 座標更新
	data_.translate = player_->worldTransform_.GetWorldPosition();
	
	// データの更新
	//RefreshData(data_);
	// 生成の制御
	UpdataEmitterFlags();

	// バッファーに送るなどの処理
	ParticleEmitter::Update();
}

void PlayerParticle::MoveParticle::Draw(ICamera* camera)
{

	ParticleEmitter::Draw(camera);

}

