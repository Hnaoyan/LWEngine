#include "Boss.h"

void Boss::Initialize(Model* model)
{
	IGameObject::Initialize(model);

	collider_.Initialize(worldTransform_.transform_.scale.x, this);
	collider_.SetAttribute(kCollisionAttributePlayer);
	worldTransform_.transform_.translate = { 0,20.0f,50.0f };
}

void Boss::Update()
{

	// 座標更新
	IGameObject::Update();
	collider_.Update(worldTransform_.GetWorldPosition());
}

void Boss::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = desc.pointLight;
	drawDesc.spotLight = desc.spotLight;
	drawDesc.worldTransform = &worldTransform_;

	model_->Draw(drawDesc);
}

void Boss::ImGuiDraw()
{

}

void Boss::OnCollision(ColliderObject target)
{
	target;
}
