#include "BossParticle.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/Particle/GPUParticleSystem.h"

void BossParticle::BulletEffect::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);
	data_.count = 2;
	data_.frequency = 1.0f/60.0f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 2;

	isBillBoard_ = true;
	blendMode_ = BlendMode::kScreen;
	RefreshData(data_);
	texture_ = TextureManager::Load("Resources/default/white2x2.png");
}

void BossParticle::BulletEffect::Update()
{
	deleteTimer_.Update();
	if (deleteTimer_.IsEnd()) {
		isDead_ = true;
	}
	// インスタンスがあるときにのみ
	if (instance_) {
		emitter_.cMap_->translate = instance_->GetWorldPosition();
	}
	else {
	}
	// 更新処理
	UpdataEmitterFlags();
	ParticleEmitter::Update();
}

void BossParticle::BulletEffect::Draw(ICamera* camera)
{
	ParticleEmitter::Draw(camera);
}
