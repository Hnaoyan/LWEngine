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
	data_.cMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	data_.cMap_->direction = { 0,-1.0f,0 };
	data_.cMap_->distance = 100.0f;
	data_.cMap_->decay = 1.0f;
	data_.cMap_->intensity = 1.0f;
	data_.cMap_->position = {};
	data_.cMap_->cosAngle = 1.0f;
}

void SpotLight::Update(const CBufferDataSpotLight& data)
{
	data_.cMap_->color = data.color;
	data_.cMap_->distance = data.distance;
	data_.cMap_->position = data.position;
	data_.cMap_->cosAngle = data.cosAngle;
	data_.cMap_->cosFalloffStart = data.cosFalloffStart;
	
	data_.cMap_->direction = Vector3::Normalize(data.direction);
	data_.cMap_->decay = std::clamp(data.decay, 0.0f, 100.0f);
	// 輝度
	data_.cMap_->intensity = std::clamp(data.intensity, 0.0f, 100.0f);
}

void SpotLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{

	ILight::Draw(cmdList, rootParamIndex);

}
