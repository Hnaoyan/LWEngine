#include "PlayerParticle.h"
#include "Application/GameObject/GameObjectLists.h"

void PlayerParticle::MoveEffect::Initialize(Model* model, uint32_t textureHandle)
{
	GPUParticleEmitter::Initialize(model, textureHandle);

	data_.count = 1;
	data_.frequency = 0.1f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = kPlayerMove;

	RefreshData(data_);

}

void PlayerParticle::MoveEffect::Update()
{
	// 座標更新
	if (player_) {
		// 生成の制御
		if (player_->GetSystemFacede()->GetParticleManager()->IsMove()) {
			UpdataEmitterFlags();
			emitter_.cMap_->emit = 1;
		}
		else {
			emitter_.cMap_->frequencyTime = 0.0f;
			emitter_.cMap_->emit = 0;
		}
		emitter_.cMap_->translate = player_->worldTransform_.GetWorldPosition();
		float offset = -0.75f;
		emitter_.cMap_->translate.y = player_->GetFootCollider()->worldTransform_.transform_.translate.y + offset;
	}

	// バッファーに送るなどの処理
	GPUParticleEmitter::Update();
}

void PlayerParticle::MoveEffect::Draw(ICamera* camera)
{

	GPUParticleEmitter::Draw(camera);

}

