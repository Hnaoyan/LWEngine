#pragma once
#include "../Utility/Singleton.h"
#include "../3D/Graphics/GraphicsPSO.h"
#include <d3d12.h>
#include <utility>

class SRVHandler;

class PostEffectRender : public Singleton<PostEffectRender>
{
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