#pragma once
#include "Engine/Base/CBufferCommon.h"
#include "../../Math/MathLib.h"
#include "../../3D/Graphics/GraphicsPSO.h"
#include "../../Base/Utility/DxCreateLib.h"

#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// 2Dスプライトクラス
/// </summary>
class Sprite
{
private:
	// リソース関数ライブラリ
	using ResourceLib = DxCreateLib::ResourceLib;
	// ブレンドモード
	using BlendMode = Pipeline::BlendMode;

public:
	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <param name="position"></param>
	/// <param name="color"></param>
	/// <param name="anchorpoint"></param>
	/// <param name="isFlipX"></param>
	/// <param name="isFlipY"></param>
	/// <returns></returns>
	static Sprite* Create(uint32_t textureHandle, Vector2 position, Vector2 anchorpoint = { 0,0 }, Vector4 color = { 1.0f,1.0f,1.0f,1.0f }, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	static void StaticInitialize(ID3D12Device* device, uint32_t width, uint32_t height);
	
	/// <summary>
	/// 描画前
	/// </summary>
	/// <param name="cmdList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後
	/// </summary>
	static void PostDraw();

private:
	// デバイス
	static ID3D12Device* sDevice_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// プロジェクションMat
	static Matrix4x4 sMatProjection_;
	// 頂点数
	static const uint32_t kVertNum = 6;

	static BlendPipeline sPipeline_;

public:
	~Sprite() = default;
	Sprite() {};
	Sprite(uint32_t textureHandle, const Vector2& position,
		const Vector2& size, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 頂点情報更新
	/// </summary>
	void TransferVertices();

private: // 固有の変数
	// リソースデスク
	D3D12_RESOURCE_DESC resourceDesc_{};

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファ
	SpriteVertexData* vertData_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> ibBuff_;
	// インデックスバッファ
	uint32_t* ibData_ = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};

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
	// テクスチャの始点
	Vector2 texBase_{ 0,0 };
	// テクスチャのサイズ
	Vector2 texSize_ = {};

	Matrix4x4 matWorld_ = {};
	EulerTransform uvTransform_ = {};

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::kNormal;

	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isInvisible_ = false;

private:
	float rotation_ = 0.0f;

public: // アクセッサ
	/// <summary>
	/// テクスチャの設定
	/// </summary>
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }

	const uint32_t GetTextureHandle() const{ return textureHandle_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) { position_ = position; }

	const Vector2& GetPosition() const{ return position_; }

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size) { size_ = size; }

	const Vector2& GetSize() const{ return size_; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(float rotation) { rotation_ = rotation; }

	const float GetRotation() const{ return rotation_; }

	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="anchorPoint"></param>
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	const Vector2 GetAnchorPoint() const{ return anchorPoint_; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { color_ = color;	}

	const Vector4 GetColor() const{ return color_; }

	/// <summary>
	/// ブレンドの設定
	/// </summary>
	/// <param name="mode"></param>
	void SetBlendMode(Pipeline::BlendMode mode) { blendMode_ = mode; }

	/// <summary>
	/// 表示か非表示かの設定
	/// </summary>
	/// <param name="isInv"></param>
	void SetInvisible(bool isInv) { isInvisible_ = isInv; }

	/// <summary>
	/// 描画サイズ設定
	/// </summary>
	/// <param name="texBase"></param>
	/// <param name="texSize"></param>
	void SetSpriteRect(const Vector2& texBase, const Vector2& texSize) {
		texBase_ = texBase;
		texSize_ = texSize;
	}

	void SetUVTransform(const EulerTransform& transform) {
		uvTransform_ = transform;
	}

	EulerTransform GetUVTransform() const{
		return uvTransform_;
	}
};