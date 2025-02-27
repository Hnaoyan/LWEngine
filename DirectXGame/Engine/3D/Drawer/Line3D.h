#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "Engine/3D/ModelUtility/ModelData.h"

#include <vector>

/// <summary>
/// 3Dラインクラス
/// </summary>
class Line3D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Intialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	/// <summary>
	/// 頂点データの作成
	/// </summary>
	void CreateVertex();
	/// <summary>
	/// データの送り直し
	/// </summary>
	void RefreshVertex();

	// 頂点のマッピングデータ
	ConstantBufferMapContext<LineData> vertex_;

	int32_t containerNum_ = 0;
public:
	//void SetVertexData(const Vector3& start, const Vector3& end) {
	//	vertexData_[containerNum_].position = start;
	//	vertexData_[containerNum_++].position = end;
	//}

public:
	// 頂点情報
	std::vector<LineData> vertexData_;

	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	size_t GetVertexSize() { return vertexData_.size(); }

};
