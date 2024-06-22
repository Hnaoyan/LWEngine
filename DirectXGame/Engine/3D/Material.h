#pragma once
#include "../Math/MathLib.h"
#include "../3D/CBuffer.h"
#include "../Light/LightData.h"
#include <wrl.h>
#include <d3d12.h>
#include <string>

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

	/// <summary>
	/// ImGui
	/// </summary>
	/// <param name="objName"></param>
	void ImGuiDraw(const std::string& objName);

public:
	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff_;
	// データ
	CBufferDataMaterial* materialData_ = nullptr;

	// 色
	Vector4 color_{ 1,1,1,1 };
	// ライティングの設定
	int32_t enableLighting_ = false;
	// 輝度
	float shininess_ = 100.0f;
	// 環境マップの係数
	float coefficient_ = 1.0f;
	// UVトランスフォーム
	EulerTransform uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

};