#include "../BossParticle.h"

void BossParticle::BulletEffect::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);
	data_.count = 1;
	data_.frequency = 0.01f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 1;

	RefreshData(data_);
}

void BossParticle::BulletEffect::Update()
{
	emitter_.cMap_->translate = instance_->GetWorldPosition();
	UpdataEmitterFlags();
	ParticleEmitter::Update();
}

void BossParticle::BulletEffect::Draw(ICamera* camera)
{
	ParticleEmitter::Draw(camera);
}
