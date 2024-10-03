#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"

#include <vector>

class Triangle3D
{
public:
	void Initialize();
	void Update();
	void Draw(ICamera* camera);
private:
	/// <summary>
	/// 頂点データの作成
	/// </summary>
	void CreateVertex();
	/// <summary>
	/// データの送り直し
	/// </summary>
	void RefreshVertex();

private:
	Vector3 vertexs_[3];
	// 頂点情報
	std::vector<TriangleData> vertexData_;
	// 頂点のマッピングデータ
	ConstantBufferMapContext<TriangleData> vertex_;

public:
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	size_t GetVertexSize() { return vertexData_.size(); }

};
