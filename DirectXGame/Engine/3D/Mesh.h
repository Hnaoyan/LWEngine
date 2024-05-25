#pragma once
#include "Drawer/Model.h"

class Mesh
{
public:
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

public:
	void CreateMesh(std::vector<VertexData> vertices);

};