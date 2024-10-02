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

private:
	// 座標コンテナ
	std::vector<Vector3> positionArray_;
	// 頂点情報
	std::vector<TrailVertex> vertexData_;

	ConstantBufferMapContext<TrailVertex> vertex_;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// 最大数
	const int kMaxBufferSize = 512;

};
