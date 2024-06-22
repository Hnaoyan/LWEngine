#include "DirectionalLight.h"
#include "../../Base/DirectXCommon.h"
#include "../../Base/Utility/DxCreateLib.h"

#include <algorithm>
#include <cassert>

DirectionalLight* DirectionalLight::CreateLight()
{
	// 生成
	DirectionalLight* instance = new DirectionalLight();
	// 初期化
	instance->Initialize();
	return instance;
}

void DirectionalLight::Initialize()
{
	// 生成の部分
	ILight::Initialize();
	// 値の初期化
	lightDataMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightDataMap_->direction = { 0.0f,-1.0f,0.0f };
	lightDataMap_->intensity = 1.0f;
}

void DirectionalLight::Update(const CBufferDataDirectionalLight& data)
{
	// それぞれの値を設定
	lightDataMap_->color = data.color;
	lightDataMap_->direction = Vector3::Normalize(data.direction);
	lightDataMap_->intensity = std::clamp(data.intensity, 0.0f, 100.0f);
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	// 描画
	ILight::Draw(cmdList, rootParamIndex);
}
