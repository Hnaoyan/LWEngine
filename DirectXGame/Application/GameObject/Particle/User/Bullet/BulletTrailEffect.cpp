#include "BulletTrailEffect.h"

void BulletParticle::TrailEffect::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);

	RefreshData(data_);
}

void BulletParticle::TrailEffect::Update()
{
	if (bullet_) {

	}

	// 更新
	ParticleEmitter::Update();
}

void BulletParticle::TrailEffect::Draw(ICamera* camera)
{

	ParticleEmitter::Draw(camera);
}