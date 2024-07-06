#include "SampleBullet.h"
#include "../../GameSystem/GameSystem.h"

void SampleBullet::Initialize(Model* model)
{
	// 基底初期化
	IGameObject::Initialize(model);
	deathTimer_.Start(120.0f);

	collider_.Initialize(1.0f, this);
	collider_.SetAttribute(kCollisionAttributePlayer);

}

void SampleBullet::Update()
{
	deathTimer_.Update();

	if (deathTimer_.isEnd_) {
		isDead_ = true;
	}

	// 移動処理
	worldTransform_.transform_.translate += velocity_ * GameSystem::GameSpeedFactor();
	// 行列更新
	IGameObject::Update();
	// コライダー更新
	collider_.Update(worldTransform_.GetWorldPosition());
}

void SampleBullet::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = desc.pointLight;
	drawDesc.spotLight = desc.spotLight;
	drawDesc.worldTransform = &worldTransform_;

	model_->Draw(drawDesc);
}

void SampleBullet::ImGuiDraw()
{

}

void SampleBullet::OnCollision(ColliderObject object)
{
	object;
}
