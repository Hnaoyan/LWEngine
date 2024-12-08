#include "BossBarrierBreakEffect.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEnginePaths.h"

void BossParticle::BarrierBreakEffect::Initialize(Model* model, uint32_t textureHandle)
{
	GPUParticleEmitter::Initialize(model, textureHandle);
	data_.count = 10;
	data_.frequency = 0.001f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = kBossBarrierBreak;
	blendMode_ = BlendMode::kAlpha;
	RefreshData(data_);
	isBillBoard_ = true;
}

void BossParticle::BarrierBreakEffect::Update()
{
	// エミッターのフラグ
	emitter_.cMap_->emit = 0;
	// ボス
	if (boss_) {
		emitter_.cMap_->translate = boss_->worldTransform_.GetWorldPosition() + boss_->HitEffectPosition();
		if (boss_->GetParticleManager()->IsBarrierBreak()) {
			perFrame_.cMap_->time += kDeltaTime;
			emitter_.cMap_->emit = 1;
		}
	}
	//UpdataEmitterFlags();
	GPUParticleEmitter::Update();
}

void BossParticle::BarrierBreakEffect::Draw(ICamera* camera)
{
	GPUParticleEmitter::Draw(camera);
}
