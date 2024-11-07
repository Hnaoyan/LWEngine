#include "BulletTrailEffect.h"

void BulletParticle::TrailEffect::Initialize(Model* model, uint32_t textureHandle)
{
	GPUParticleEmitter::Initialize(model, textureHandle);

	RefreshData(data_);
}

void BulletParticle::TrailEffect::Update()
{
	if (bullet_) {

	}

	// 更新
	GPUParticleEmitter::Update();
}

void BulletParticle::TrailEffect::Draw(ICamera* camera)
{

	GPUParticleEmitter::Draw(camera);
}
