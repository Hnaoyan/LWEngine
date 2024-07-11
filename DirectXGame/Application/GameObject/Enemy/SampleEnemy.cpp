#include "SampleEnemy.h"

void SampleEnemy::Initialize(Model* model)
{
	// 基底初期化
	IGameObject::Initialize(model);
	// 初期化
	collider_.Initialize(1.0f, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);
}

void SampleEnemy::Update()
{
	// 行列更新
	IGameObject::Update();
	// コライダー更新
	collider_.Update(worldTransform_.GetWorldPosition());
}

void SampleEnemy::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = desc.pointLight;
	drawDesc.spotLight = desc.spotLight;
	drawDesc.worldTransform = &worldTransform_;

	model_->Draw(drawDesc);
}

void SampleEnemy::ImGuiDraw()
{
}

void SampleEnemy::OnCollision(ColliderObject object)
{
	if (std::holds_alternative<SampleBullet*>(object)) {
		isDead_ = true;
	}

}
