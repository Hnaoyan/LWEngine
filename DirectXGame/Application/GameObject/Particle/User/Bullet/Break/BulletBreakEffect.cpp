#include "BulletBreakEffect.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/DeltaTime.h"

void BulletParticle::BreakEffect::Initialize(Model* model, uint32_t textureHandle)
{
	GPUParticleEmitter::Initialize(model, textureHandle);
	data_.count = 10;
	data_.frequency = 0.1f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 4;
	blendMode_ = Pipeline::BlendMode::kAlpha;

	texture_ = TextureManager::Load("Resources/Effect/effect.png");

	RefreshData(data_);
	isBillBoard_ = true;
}

void BulletParticle::BreakEffect::Update()
{
	// フラグ初期化
	//emitter_.cMap_->emit = 0;
	perFrame_.cMap_->time += kDeltaTime;

	//RefreshData(data_);
	GPUParticleEmitter::Update();
	emitter_.cMap_->emit = 0;
}

void BulletParticle::BreakEffect::Draw(ICamera* camera)
{
	GPUParticleEmitter::Draw(camera);
}
