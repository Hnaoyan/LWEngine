#pragma once
#include "Engine/Base/CBufferCommon.h"

/// <summary>
/// トレイル用マテリアル
/// </summary>
class TrailMaterial
{
public:
	void CreateMaterial();
	void Update();
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> GetCBuffer() { return buffer_.cBuffer; }
	void SetUVTransform(const EulerTransform& transform) { uvTransform_ = transform; }
private:
	// バッファ
	ConstantBufferMapContext<CBufferDataTrailMaterial> buffer_;
	// UV
	EulerTransform uvTransform_;
	// 色
	Vector4 color_;
	float discardThreshold_;
};
