#pragma once
#include "../../Base/CBufferCommon.h"
#include "../ModelData.h"

#include <vector>

class MissileTrail
{
public:

	//void Update();

private:
	// 座標コンテナ
	std::vector<Vector3> positionArray_;
	// 頂点情報
	std::vector<VertexData> vertexData_;

};
