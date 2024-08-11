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
	data_.cMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	data_.cMap_->direction = { 0.0f,-1.0f,0.0f };
	data_.cMap_->intensity = 1.0f;
}

void DirectionalLight::Update(const CBufferDataDirectionalLight& data)
{
	// それぞれの値を設定
	data_.cMap_->color = data.color;
	data_.cMap_->direction = Vector3::Normalize(data.direction);
	data_.cMap_->intensity = std::clamp(data.intensity, 0.0f, 100.0f);
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	// 描画
	ILight::Draw(cmdList, rootParamIndex);
}
