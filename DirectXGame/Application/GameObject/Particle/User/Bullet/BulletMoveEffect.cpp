#include "BulletMoveEffect.h"
#include "Application/GameObject/Bullet/IBullet.h"
#include "Engine/2D/TextureManager.h"

void BulletParticle::MoveEffect::Initialize(Model* model, uint32_t textureHandle)
{
	ParticleEmitter::Initialize(model, textureHandle);
	data_.count = 4;
	data_.frequency = 1.0f;
	data_.frequencyTime = 0.0f;
	data_.translate = {};
	data_.radius = 1.0f;
	data_.emit = 0;
	data_.emitPattern = 2;

	blendMode_ = ParticleEmitter::BlendMode::kAlpha;

	texture_ = TextureManager::Load("Resources/Effect/test.png");
	RefreshData(data_);

}

void BulletParticle::MoveEffect::Update()
{
	// 生成の制御
	UpdataEmitterFlags();
	// 座標更新
	if (bullet_) {
		emitter_.cMap_->emit = 1;
		//emitter_.cMap_->frequencyTime = 0.0f;
		emitter_.cMap_->translate = bullet_->GetWorldPosition();
	}

	// バッファーに送るなどの処理
	ParticleEmitter::Update();
}

void BulletParticle::MoveEffect::Draw(ICamera* camera)
{

	ParticleEmitter::Draw(camera);

}

