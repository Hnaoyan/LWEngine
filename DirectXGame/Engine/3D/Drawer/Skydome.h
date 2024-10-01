#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "../ModelUtility/ModelData.h"
#include "../WorldTransform.h"

class Skydome
{
public:
	/// <summary>
	/// 作成
	/// </summary>
	/// <returns></returns>
	static Skydome* CreateSkydome();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();
	/// <summary>
	/// マテリアル作成
	/// </summary>
	void CreateMaterial();

	/// <summary>
	/// Sphereの描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(const ModelDrawDesc& desc);

private:
	// 分割数
	uint32_t subdivision_ = 0;
	// 頂点数
	uint32_t vertexIndex_ = 0;

	// 緯度（横
	uint32_t latIndex_ = 0;
	// 経度（縦
	uint32_t lonIndex_ = 0;

	// テクスチャ
	uint32_t textureHandle_ = 0u;

private: // メッシュ
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// 頂点バッファマップ
	VertexData* vertData_ = nullptr;

private: // マテリアル
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
	// UVトランスフォーム
	EulerTransform uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

};
