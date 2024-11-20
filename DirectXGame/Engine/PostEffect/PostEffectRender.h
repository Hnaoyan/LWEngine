#pragma once
#include "../Utility/Singleton.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "PostEffectData.h"
#include "Engine/Base/CBufferCommon.h"

#include <wrl.h>
#include <d3d12.h>
#include <utility>

class SRVHandler;
/// <summary>
/// ポストエフェクト描画クラス
/// </summary>
class PostEffectRender : public Singleton<PostEffectRender>
{
private:
	// レジスター省略
	using EffectRegister = Pipeline::PostEffectRegister;

public:	// 定数バッファ
	// ビネット
	ConstantBufferMapContext<CBufferDataVignette> vignette_;
	// ブラー
	ConstantBufferMapContext<CBufferDataBlur> blur_;
	// ディゾルブ
	ConstantBufferMapContext<CBufferDataDissolve> dissolve_;
	// ノイズ
	ConstantBufferMapContext<CBufferDataNoise> noise_;
	// HSV
	ConstantBufferMapContext<CBufferDataHSV> hsv_;
	// ブルーム
	ConstantBufferMapContext<CBufferDataBloom> bloom_;
	/// <summary>
	/// 初期化
	/// </summary>
	void StaticInitialize();

public:
	// ポストエフェクトのバッファーに送るデータ
	struct PostEffectDesc {
		CBufferDataBlur blur;
		CBufferDataVignette vignette;
		CBufferDataDissolve dissolve;
		CBufferDataNoise noise;
		CBufferDataHSV hsv;
		CBufferDataBloom bloom;
	};
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="desc"></param>
	void Update(const PostEffectDesc& desc);

public:
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	ID3D12Device* device_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;


public:
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> renderTextureHandle_;
	uint32_t srvIndex_ = 0u;

	static Pipeline::PostEffectType sPostEffect;
	static uint32_t sDissolveTexture;
};