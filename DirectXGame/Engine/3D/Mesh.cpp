#include "Mesh.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

void Mesh::CreateMesh(ModelData* modelData)
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点
	{
		// バッファ作成
		vertex_.CreateConstantBuffer(device, modelData->vertices.size());
		std::memcpy(vertex_.cMap_, modelData->vertices.data(), sizeof(VertexData) * modelData->vertices.size());
		// ビュー作成
		vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(VertexData) * modelData->vertices.size());
		vbView_.StrideInBytes = sizeof(VertexData);
	}

	if (modelData->indices.empty()) {
		return;
	}
	// インデックス
	{
		// バッファ作成
		index_.CreateConstantBuffer(device, modelData->indices.size());
		std::memcpy(index_.cMap_, modelData->indices.data(), sizeof(uint32_t) * modelData->indices.size());
		// ビュー作成
		ibView_.BufferLocation = index_.cBuffer->GetGPUVirtualAddress();
		ibView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData->indices.size());
		ibView_.Format = DXGI_FORMAT_R32_UINT;
	}
}

void Mesh::CreateMeshObj(ModelData* modelData)
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点
	{
		// バッファ
		vertex_.CreateConstantBuffer(device, modelData->vertices.size());
		std::memcpy(vertex_.cMap_, modelData->vertices.data(), sizeof(VertexData) * modelData->vertices.size());
		// ビュー
		vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(VertexData) * modelData->vertices.size());
		vbView_.StrideInBytes = sizeof(VertexData);
	}
}
