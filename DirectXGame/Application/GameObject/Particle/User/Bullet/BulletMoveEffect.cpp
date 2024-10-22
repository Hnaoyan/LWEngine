#include "BulletMoveEffect.h"
#include "Application/GameObject/Bullet/IBullet.h"

void BulletParticle::MoveEffect::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);
	data_.count = 1;
	data_.frequency = 0.1f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 2;

	RefreshData(data_);

}

void BulletParticle::MoveEffect::Update()
{
	// 座標更新
	if (bullet_) {
		// 生成の制御
		UpdataEmitterFlags();
		emitter_.cMap_->emit = 1;
		emitter_.cMap_->frequencyTime = 0.0f;
		emitter_.cMap_->translate = bullet_->GetWorldPosition();
	}

	// バッファーに送るなどの処理
	ParticleEmitter::Update();
}

void BulletParticle::MoveEffect::Draw(ICamera* camera)
{

	ParticleEmitter::Draw(camera);

}

