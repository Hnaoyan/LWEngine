#pragma once
#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include "../Base/CBufferCommon.h"
#include "ModelUtility/ModelData.h"

class Mesh
{
public:
	/// <summary>
	/// メッシュ生成
	/// </summary>
	/// <param name="vertices"></param>
	void CreateMesh(ModelData* modelData);

	void CreateMeshObj(ModelData* modelData);

public:
	// 頂点バッファ
	ConstantBufferMapContext<VertexData> vertex_;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// インデックス
	ConstantBufferMapContext<uint32_t> index_;
	D3D12_INDEX_BUFFER_VIEW ibView_{};

};