#pragma once
#include "Engine/Base/CBufferCommon.h"

class TrailMaterial
{
public:
	void CreateMaterial();
	void Update();
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> GetCBuffer() { return buffer_.cBuffer; }
private:
	// バッファ
	ConstantBufferMapContext<CBufferDataTrailMaterial> buffer_;
	// UV
	EulerTransform uvTransform_;
	// 色
	Vector4 color_;
};
