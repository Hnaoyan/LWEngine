#pragma once
#include "../../Math/MathLib.h"
#include "../../3D/CBuffer.h"
#include "../../Base/Utility/DxCreateLib.h"

#include <wrl.h>
#include <d3d12.h>

class Sprite
{
private:
	using ResourceLib = DxCreateLib::ResourceLib;

public:
	static Sprite* Create(uint32_t textureHandle, Vector2 position, Vector4 color = { 1.0f,1.0f,1.0f,1.0f }, Vector2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	static void StaticInitialize(ID3D12Device* device, uint32_t width, uint32_t height);
	
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

private:
	// デバイス
	static ID3D12Device* sDevice_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// プロジェクションMat
	static Matrix4x4 sMatProjection_;
	// 頂点数
	static const uint32_t kVertNum = 4;

public:
	~Sprite() = default;
	Sprite() {};
	Sprite(uint32_t textureHandle, const Vector2& position,
		const Vector2& size, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY);

	bool Initialize();

	void Draw() {};

	void TransferVertices() {};

private: // 固有の変数
	// リソースデスク
	D3D12_RESOURCE_DESC resourceDesc_{};

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファ
	VertexData* vertData_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};


	//---ここは後で実装---//
	//// インデックスバッファビュー
	//D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;

private: // メンバ変数
	// 定数バッファ
	SpriteDataForGPU* spriteGPUData_ = nullptr;
	// 定数リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteGPUBuff_;

	uint32_t textureHandle_ = 0u;

	Vector2 position_ = {};
	Vector2 size_ = {};
	Vector2 anchorPoint_ = {};
	Vector4 color_ = {};
	Vector2 texSize_ = {};

	Matrix4x4 matWorld_ = {};

	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isInvisible_ = false;

};