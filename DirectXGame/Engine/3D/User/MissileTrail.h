#pragma once
#include "../../Base/CBufferCommon.h"
#include "../ModelUtility/ModelData.h"

#include <vector>

class MissileTrail
{
public:

	void Update();

	void Draw();

private:
	// 座標コンテナ
	std::vector<Vector3> positionArray_;
	// 頂点情報
	std::vector<VertexData> vertexData_;

	// 最大数
	const int kMaxBufferSize = 512;

};
