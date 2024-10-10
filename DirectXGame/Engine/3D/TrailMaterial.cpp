#include "TrailMaterial.h"
#include "../Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"

void TrailMaterial::CreateMaterial()
{
	// 色
	color_ = Vector4::DefaultOnes();

	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	// リソース作成
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// マテリアルバッファの生成
	buffer_.CreateConstantBuffer(device);

}

void TrailMaterial::Update()
{
	// データの転送
	buffer_.cMap_->color = color_;
	buffer_.cMap_->uvTransform = Matrix4x4::MakeUvTransformMatirx(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
}
