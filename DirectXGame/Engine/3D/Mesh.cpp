#include "Mesh.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

void Mesh::CreateMesh(std::vector<VertexData> vertices)
{
	//HRESULT result = S_FALSE;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点バッファの生成
	vertBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(VertexData) * vertices.size());

	// ビュー作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
	vbView_.StrideInBytes = sizeof(VertexData);

	// データを書き込む
	VertexData* vertexData = nullptr;
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, vertices.data(), sizeof(VertexData) * vertices.size());

}
