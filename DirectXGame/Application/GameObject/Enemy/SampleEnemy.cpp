#include "SampleEnemy.h"

void SampleEnemy::Initialize(Model* model)
{
	// 基底初期化
	IGameObject::Initialize(model);
	// 初期化
	collider_.Initialize(1.0f, this);
	collider_.SetAttribute(kCollisionAttributeEnemy);

	timer_.Start(120.0f);
	isLeft_ = false;
}

void SampleEnemy::Update()
{
	if (isLeft_) {
		worldTransform_.transform_.translate.x -= 0.1f;
		worldTransform_.transform_.translate.y -= 0.1f;
	}
	else {
		worldTransform_.transform_.translate.x += 0.1f;
		worldTransform_.transform_.translate.y += 0.1f;
	}

	if (timer_.IsEnd()) {
		if (isLeft_) {
			isLeft_ = false;
		}
		else {
			isLeft_ = true;
		}

		timer_.Start(120.0f);
	}

	timer_.Update(1.0f);
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
