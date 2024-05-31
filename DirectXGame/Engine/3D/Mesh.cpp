#include "Mesh.h"
#include "../Base/DirectXCommon.h"
#include "../Base/Utility/DxCreateLib.h"

void Mesh::CreateMesh(std::vector<VertexData> vertices)
{
	HRESULT result = S_FALSE;
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点
	{
		// 頂点バッファの生成
		vertBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(VertexData) * vertices.size());

		// ビュー作成
		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
		vbView_.StrideInBytes = sizeof(VertexData);
	}

	// インデックス
	{
		// リソース設定
		ibBuff_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(uint32_t) * 6);

		// マッピング
		result = ibBuff_->Map(0, nullptr, reinterpret_cast<void**>(&ibData_));
		assert(SUCCEEDED(result));

		ibData_[0] = 0;
		ibData_[1] = 1;
		ibData_[2] = 2;
		ibData_[3] = 1;
		ibData_[4] = 3;
		ibData_[5] = 2;

		ibView_.BufferLocation = ibBuff_->GetGPUVirtualAddress();
		ibView_.SizeInBytes = sizeof(uint32_t) * 6;
		ibView_.Format = DXGI_FORMAT_R32_UINT;
	}

	// データを書き込む
	//VertexData* vertexData = nullptr;
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertData_));
	std::memcpy(vertData_, vertices.data(), sizeof(VertexData) * vertices.size());

}
