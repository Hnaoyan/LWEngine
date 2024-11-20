#include "BulletMoveEffect.h"
#include "Application/GameObject/Bullet/IBullet.h"
#include "Engine/2D/TextureManager.h"

void BulletParticle::MoveEffect::Initialize(Model* model, uint32_t textureHandle)
{
	GPUParticleEmitter::Initialize(model, textureHandle);
	emitter_.cMap_->count = 4;
	emitter_.cMap_->frequency = 1.0f / 60.0f;
	emitter_.cMap_->frequencyTime = 0.0f;
	emitter_.cMap_->translate = {};
	emitter_.cMap_->radius = 1.0f;
	emitter_.cMap_->emit = 0;
	emitter_.cMap_->emitPattern = 2;

	// ブレンドモード
	blendMode_ = GPUParticleEmitter::BlendMode::kAlpha;
	// テクスチャ
	texture_ = TextureManager::Load("Resources/Effect/Smoke3.png");
	// データの更新
	//RefreshData(data_);
}

void BulletParticle::MoveEffect::Update()
{
	// 生成の制御
	UpdataEmitterFlags();
	// 座標更新
	if (bullet_) {
		assert(bullet_);
		if (!bullet_->IsDead()) {
			//emitter_.cMap_->translate = bullet_->GetWorldPosition();
			if (trail_->GetBeginPoint() == Vector3::Zero()) {
				emitter_.cMap_->emit = 0;
			}
			emitter_.cMap_->translate = trail_->GetBeginPoint();
			
		}
		else {
			emitter_.cMap_->emit = 0;
		}
		//emitter_.cMap_->emit = 1;
		//emitter_.cMap_->frequencyTime = 0.0f;
		//assert(bullet_->GetWorldPosition());
		//emitter_.cMap_->translate = trail_->GetBeginPoint();
		//if (bullet_->IsDead()) {
		//	emitter_.cMap_->emit = 0;
		//}
	}
	else {
		emitter_.cMap_->emit = 0;
	}

	// バッファーに送るなどの処理
	GPUParticleEmitter::Update();
}

void BulletParticle::MoveEffect::Draw(ICamera* camera)
{

	GPUParticleEmitter::Draw(camera);

}

