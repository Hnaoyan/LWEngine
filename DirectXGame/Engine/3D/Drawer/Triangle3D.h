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

	void GenerateVertices();

	void GeneratePolygon(const Vector4& color, const float& width);
	void UpdateVertex(const Vector4& color, const float& width);

private:
	Vector3 vertexs_[3];

	// カーブ用の座標
	std::vector<Vector3> curvePoints_;

	// 頂点情報
	std::vector<TriangleData> vertexData_;
	// 頂点のマッピングデータ
	ConstantBufferMapContext<TriangleData> vertex_;

	// インデックス
	ConstantBufferMapContext<uint32_t> index_;
	std::vector<uint32_t> indices_;

	uint32_t texture_ = 0u;

public:
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	size_t GetVertexSize() { return vertexData_.size(); }
	size_t GetIndexSize() { return indices_.size(); }

};
