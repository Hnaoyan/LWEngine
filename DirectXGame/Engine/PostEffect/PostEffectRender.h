#pragma once
#include "../Utility/Singleton.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include "PostEffectData.h"

#include <wrl.h>
#include <d3d12.h>
#include <utility>

class SRVHandler;

class PostEffectRender : public Singleton<PostEffectRender>
{
private:
	using EffectRegister = Pipeline::PostEffectRegister;

public:	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vignetteCBuffer_;
	CBufferDataVignette* vignetteMap_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> blurCBuffer_;
	CBufferDataBlur* blurMap_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void StaticInitialize();

public:
	// ポストエフェクトのバッファーに送るデータ体
	struct PostEffectDesc {
		CBufferDataBlur blur;
		CBufferDataVignette vignette;
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
};