#include "Mesh.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

void Mesh::CreateMesh(ModelData* modelData)
{
	//HRESULT result = S_FALSE;
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点
	{
		// 頂点バッファの生成
		vertBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(VertexData) * modelData->vertices.size());

		// ビュー作成
		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(VertexData) * modelData->vertices.size());
		vbView_.StrideInBytes = sizeof(VertexData);

		// データ書き込み
		vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
		std::memcpy(vertData_, modelData->vertices.data(), sizeof(VertexData) * modelData->vertices.size());
	}

	// インデックス
	{
		// リソース設定
		ibBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(uint32_t) * modelData->indices.size());

		// ビュー作成
		ibView_.BufferLocation = ibBuff_->GetGPUVirtualAddress();
		ibView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData->indices.size());
		ibView_.Format = DXGI_FORMAT_R32_UINT;

		// マッピング
		ibBuff_->Map(0, nullptr, reinterpret_cast<void**>(&ibData_));
		std::memcpy(ibData_, modelData->indices.data(), sizeof(uint32_t) * modelData->indices.size());
	}
}
