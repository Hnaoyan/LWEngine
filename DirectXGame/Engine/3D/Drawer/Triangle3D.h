#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"

#include <vector>
#include <optional>

class Triangle3D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 頂点更新
	/// </summary>
	/// <param name="controlPoint"></param>
	void Update(std::vector<Vector3> controlPoint);
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
	/// <summary>
	/// 頂点バッファに送る
	/// </summary>
	void TransferVertex();

private:
	// 頂点
	ConstantBufferMapContext<TriangleData> vertex_;
	std::vector<TriangleData> vertexData_;
	// インデックス
	ConstantBufferMapContext<uint32_t> index_;
	std::vector<uint32_t> indices_;
private:
	// カメラ
	std::optional<ICamera*> camera_;
	// 色
	Vector4 color_{};
	// 三角形の幅
	float width_ = 0.0f;

public:
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	uint32_t texture_ = 0u;

	bool isBillBoard_ = false;
public:
	// セッター
	void SetWidth(float width) { width_ = width; }
	void SetColor(const Vector4& color) { color_ = color; }
	void SetCamera(ICamera* camera) { camera_.emplace(camera); }
	// ゲッター
	bool IsCamera() { return camera_.has_value(); }
	size_t GetVertexSize() { return vertexData_.size(); }
	size_t GetIndexSize() { return indices_.size(); }

};
