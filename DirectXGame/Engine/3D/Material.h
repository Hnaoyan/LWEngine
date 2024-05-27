#pragma once
#include "../Math/MathLib.h"
#include "../3D/CBuffer.h"
#include "../Light/LightData.h"
#include <wrl.h>
#include <d3d12.h>

class Material
{
public:
	/// <summary>
	/// マテリアルの生成
	/// </summary>
	void CreateMaterial();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:
	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff_;
	// データ
	CBufferDataMaterial* materialData_ = nullptr;

	// 色
	Vector4 color_;
	// ライティングの設定
	int32_t enableLighting_;
	// 輝度
	float shininess_;

};