#include "SpotLight.h"
#include <algorithm>

SpotLight* SpotLight::CreateLight()
{
	// 生成
	SpotLight* instance = new SpotLight();
	// 初期化
	instance->Initialize();
	return instance;
}

void SpotLight::Initialize()
{
	ILight::Initialize();
	lightDataMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightDataMap_->direction = { 0,-1.0f,0 };
	lightDataMap_->distance = 100.0f;
	lightDataMap_->decay = 1.0f;
	lightDataMap_->intensity = 1.0f;
	lightDataMap_->position = {};
	lightDataMap_->cosAngle = 1.0f;
}

void SpotLight::Update(const CBufferDataSpotLight& data)
{
	lightDataMap_->color = data.color;
	lightDataMap_->distance = data.distance;
	lightDataMap_->position = data.position;
	lightDataMap_->cosAngle = data.cosAngle;
	lightDataMap_->cosFalloffStart = data.cosFalloffStart;
	
	lightDataMap_->direction = Vector3::Normalize(data.direction);
	lightDataMap_->decay = std::clamp(data.decay, 0.0f, 100.0f);
	// 輝度
	lightDataMap_->intensity = std::clamp(data.intensity, 0.0f, 100.0f);
}

void SpotLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{

	ILight::Draw(cmdList, rootParamIndex);

}
