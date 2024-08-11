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
	data_.cMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	data_.cMap_->position = { 0,-1.0f,0 };
	data_.cMap_->decay = 1.0f;
	data_.cMap_->radius = 1.0f;
	data_.cMap_->intensity = 1.0f;
}

void PointLight::Update(const CBufferDataPointLight& data)
{
	data_.cMap_->color = data.color;
	data_.cMap_->position = data.position;
	data_.cMap_->radius = data.radius;
	// 減衰率
	data_.cMap_->decay = std::clamp(data.decay, 0.0f, 100.0f);
	// 輝度
	data_.cMap_->intensity = std::clamp(data.intensity, 0.0f, 100.0f);
}

void PointLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	ILight::Draw(cmdList, rootParamIndex);
}
