#include "Material.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

void Material::CreateMaterial()
{
	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// 無し
	enableLighting_ = kNone;

	shininess_ = 100.0f;

	// リソース作成
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// マテリアルバッファの生成
	materialBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(CBufferDataMaterial));

	materialBuff_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	// データの転送
	materialData_->color = color_;
	materialData_->enableLighting = enableLighting_;
	
}

void Material::Update()
{
	// データの転送
	materialData_->color = color_;
	materialData_->enableLighting = enableLighting_;

}
