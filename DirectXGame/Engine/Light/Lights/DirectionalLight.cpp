#include "DirectionalLight.h"
#include "../../Base/DirectXCommon.h"
#include "../../Base/Utility/DxCreateLib.h"

#include <cassert>

ID3D12GraphicsCommandList* DirectionalLight::sCommandList = nullptr;

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
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 平行光源リソースを作る
	directionalLightBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(CBufferDataDirectionalLight));

	// 書き込むためのアドレスを取得
	directionalLightBuff_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightMap_));

	// 仮のデフォルト値
	directionalLightMap_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightMap_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightMap_->intensity = 1.0f;
}

void DirectionalLight::Update(const CBufferDataDirectionalLight& data)
{
	directionalLightMap_->color = data.color;
	directionalLightMap_->direction = data.direction;
	directionalLightMap_->intensity = data.intensity;
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)
{
	// チェック
	assert(sCommandList == nullptr);
	// コマンドリストの登録
	sCommandList = cmdList;
	// CBufferの設定
	sCommandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(rootParamIndex), directionalLightBuff_->GetGPUVirtualAddress());
	// コマンドリストの解除
	sCommandList = nullptr;
}
