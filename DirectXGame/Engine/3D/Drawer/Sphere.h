#pragma once
#include "../CBuffer.h"
#include "../ModelData.h"
#include "../WorldTransform.h"

class Sphere
{
public:
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;

public:
	static Sphere* CreateSphere();

	void Initialize();

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();
	/// <summary>
	/// マテリアル作成
	/// </summary>
	void CreateMaterial();

	void Draw(const ModelDrawDesc& desc);

private:
	// 分割数
	int32_t subdivision_ = 0;
	// 頂点数
	int32_t vertexIndex_ = 0;

	// 緯度（横
	int32_t latIndex_ = 0;
	// 経度（縦
	int32_t lonIndex_ = 0;

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

};
