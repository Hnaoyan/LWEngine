#include "Sphere.h"
#include "../../Base/DirectXCommon.h"
#include "../../Base/Utility/DxCreateLib.h"

Sphere* Sphere::CreateSphere()
{
    // メモリ確保
    Sphere* instance = new Sphere;
    // 初期化
    instance->Initialize();

    return instance;
}

void Sphere::Initialize()
{
	// 分割数	
	subdivision_ = 512;
	// 頂点数
	vertexIndex_ = subdivision_ * subdivision_ * 6;

	// 頂点作成
	CreateVertex();
	// マテリアル作成
	CreateMaterial();

}

void Sphere::CreateVertex()
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点
	{
		// 頂点バッファの生成
		vertBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(VertexData) * vertexIndex_);

		// ビュー作成
		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(VertexData) * vertexIndex_);
		vbView_.StrideInBytes = sizeof(VertexData);

		// データ書き込み
		vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
		//std::memcpy(vertData_, modelData->vertices.data(), sizeof(VertexData) * modelData->vertices.size());
	}
}

void Sphere::CreateMaterial()
{
	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	// 無し
	enableLighting_ = false;

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
