#include "PointLight.h"
#include <algorithm>

PointLight* PointLight::CreateLight()
{
	// 生成
	PointLight* instance = new PointLight();
	// 初期化
	instance->Initialize();
	return instance;
}

void PointLight::Initialize()
{
	ILight::Initialize();
	lightDataMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightDataMap_->position = { 0,-1.0f,0 };
	lightDataMap_->decay = 1.0f;
	lightDataMap_->radius = 1.0f;
	lightDataMap_->intensity = 1.0f;
}

void PointLight::Update(const CBufferDataPointLight& data)
{
	lightDataMap_->color = data.color;
	lightDataMap_->position = data.position;
	lightDataMap_->radius = data.radius;
	// 減衰率
	lightDataMap_->decay = std::clamp(data.decay, 0.0f, 100.0f);
	// 輝度
	lightDataMap_->intensity = std::clamp(data.intensity, 0.0f, 100.0f);
}

void PointLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	ILight::Draw(cmdList, rootParamIndex);
}
