#pragma once
#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include "../3D/CBuffer.h"


class Mesh
{
public:
	/// <summary>
	/// メッシュ生成
	/// </summary>
	/// <param name="vertices"></param>
	void CreateMesh(std::vector<VertexData> vertices);

public:
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// 頂点バッファマップ
	VertexData* vertData_ = nullptr;

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

};