#include "SampleBullet.h"

void SampleBullet::Initialize(Model* model)
{
	// 基底初期化
	IGameObject::Initialize(model);
}

void SampleBullet::Update()
{

	// 行列更新
	IGameObject::Update();
}

void SampleBullet::Draw(ModelDrawDesc desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.pointLight = 
}

void SampleBullet::ImGuiDraw()
{

}
