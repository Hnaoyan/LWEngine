#include "BossParticle.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/DeltaTime.h"

void BossParticle::DamageEffect::Initialize(Model* model, uint32_t textureHandle)
{
	GPUParticleEmitter::Initialize(model, textureHandle);
	data_.count = 5;
	data_.frequency = 0.001f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 3;
	blendMode_ = BlendMode::kAlpha;
	RefreshData(data_);
	isBillBoard_ = true;
}

void BossParticle::DamageEffect::Update()
{
	// エミッターのフラグ
	emitter_.cMap_->emit = 0;
	// ボス
	if (boss_) {
		emitter_.cMap_->translate = boss_->worldTransform_.GetWorldPosition();
		if (boss_->GetParticleManager()->IsDamage()) {
			perFrame_.cMap_->time += kDeltaTime;
			emitter_.cMap_->emit = 1;
		}
	}
	//UpdataEmitterFlags();
	GPUParticleEmitter::Update();
}

void BossParticle::DamageEffect::Draw(ICamera* camera)
{
	GPUParticleEmitter::Draw(camera);
}
