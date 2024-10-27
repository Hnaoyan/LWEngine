#include "BulletBreakEffect.h"

void BulletParticle::BreakEffect::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);
	data_.count = 10;
	data_.frequency = 0.1f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 3;
	blendMode_ = Pipeline::BlendMode::kAdd;

	RefreshData(data_);
	isBillBoard_ = true;
}

void BulletParticle::BreakEffect::Update()
{
	// フラグ初期化
	emitter_.cMap_->emit = 0;


	ParticleEmitter::Update();
}

void BulletParticle::BreakEffect::Draw(ICamera* camera)
{
	ParticleEmitter::Draw(camera);
}
