#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"
#include "Engine/3D/TrailMaterial.h"

#include <vector>
#include <optional>

class Trail3D
{
public:
	// コンストラクタ
	Trail3D();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="controlPoint"></param>
	void Update(std::vector<Vector3> controlPoint);

private:
	// 頂点更新
	void TransferVertex();
	// 頂点バッファ生成
	void CreateVertex();
	// 座標から頂点作成
	void BuildVertexFromPoints(const std::vector<Vector3>& points);
private:
	// 頂点
	ConstantBufferMapContext<TriangleData> vertex_;
	std::vector<TriangleData> vertexData_;
	// インデックス
	ConstantBufferMapContext<uint32_t> index_;
	std::vector<uint32_t> indices_;

	// マテリアル
	std::unique_ptr<TrailMaterial> material_;
	
private:
	// カメラ
	std::optional<ICamera*> camera_;
	// 色
	Vector4 color_{};
	// 三角形の幅
	float width_ = 0.0f;
	float maxAlpha_ = 1.0f;

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
	TrailMaterial* GetMaterial() { return material_.get(); }

};
