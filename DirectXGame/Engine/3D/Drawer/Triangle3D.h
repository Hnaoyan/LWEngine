#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"

#include <vector>

class Triangle3D
{
public:
	void Initialize();
	void Update(std::vector<Vector3> controlPoint);
	void SetCamera(ICamera* camera) { camera_ = camera; }
private:
	/// <summary>
	/// 頂点データの作成
	/// </summary>
	void CreateVertex();
	/// <summary>
	/// データの送り直し
	/// </summary>
	void RefreshVertex();

	/// <summary>
	/// 頂点情報の更新
	/// </summary>
	/// <param name="controllPoint"></param>
	/// <param name="color"></param>
	/// <param name="width"></param>
	void UpdateVertex(std::vector<Vector3> controllPoint, const Vector4& color, const float& width);

private:

private:
	// 頂点
	ConstantBufferMapContext<TriangleData> vertex_;
	std::vector<TriangleData> vertexData_;
	// インデックス
	ConstantBufferMapContext<uint32_t> index_;
	std::vector<uint32_t> indices_;
public:
	// カメラ
	ICamera* camera_ = nullptr;

public:
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	size_t GetVertexSize() { return vertexData_.size(); }
	size_t GetIndexSize() { return indices_.size(); }

	bool isBillBoard_ = false;

};
