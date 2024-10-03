#pragma once
#include "../../Base/CBufferCommon.h"
#include "../ModelUtility/ModelData.h"

#include <vector>

class MissileTrail
{
public:
	void Initialize();

	void Update();

	void Draw(ICamera* camera);

	void CreateVertex();

	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return vbView_; }
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	size_t GetVertexSize() { return vertexData_.size(); }
private:
	void RefreshVertex();

private:
	// 座標コンテナ
	std::vector<Vector3> positionArray_;
	// 頂点情報
	std::vector<TrailVertex> vertexData_;

	ConstantBufferMapContext<TrailVertex> vertex_;

	// 最大数
	const int kMaxBufferSize = 512;

};
