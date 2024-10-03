#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"

#include <vector>

class Line3D
{
public:
	void Intialize();
	void Update();
	void Draw(ICamera* camera);

private:
	void CreateVertex();
	void RefreshVertex();

	// 頂点情報
	std::vector<LineData> vertexData_;
	// 頂点のマッピングデータ
	ConstantBufferMapContext<LineData> vertex_;
public:
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	size_t GetVertexSize() { return vertexData_.size(); }

};
