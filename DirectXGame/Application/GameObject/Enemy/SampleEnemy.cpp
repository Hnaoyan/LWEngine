#include "SampleEnemy.h"

void SampleEnemy::Initialize(Model* model)
{
	// 基底初期化
	IGameObject::Initialize(model);
}

void SampleEnemy::Update()
{
	// 行列更新
	IGameObject::Update();
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
	object;
}
