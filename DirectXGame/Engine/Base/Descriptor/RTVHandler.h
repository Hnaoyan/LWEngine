#pragma once
#include "../../Utility/Singleton.h"
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <vector>

class DirectXDevice;
class SwapChainManager;

/// <summary>
/// RTV管理
/// </summary>
class RTVHandler : public Singleton<RTVHandler>
{
public:
	// 次の番号
	static uint32_t sNextIndexDescriptorHeap_;

public: // 初期化時のみ
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dxDevice"></param>
	/// <param name="swapChainManager"></param>
	void StaticInitialize(DirectXDevice* dxDevice, SwapChainManager* swapChainManager);

public:
	/// <summary>
	/// RTVのクリア
	/// </summary>
	/// <param name="cmdList"></param>
	void ClearRenderTarget(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// PostEffectTextureのクリア
	/// </summary>
	/// <param name="cmdList"></param>
	void ClearRenderTexture(ID3D12GraphicsCommandList* cmdList);

public: // アクセッサ
	// RTVDesc
	D3D12_RENDER_TARGET_VIEW_DESC GetDesc() { return rtvDesc_; }
	// BackBuffer
	ID3D12Resource* GetBackBuffer(UINT index) { return backBuffer_[index].Get(); }
	// RenderTexture
	ID3D12Resource* GetRenderTexture() { return renderTextureResource_.Get(); }
	// BufferCount
	size_t GetBackBufferCount() const { return backBuffer_.size(); }
	// Heap取得
	ID3D12DescriptorHeap* GetHeap() { return rtvHeap_.Get(); }
	// ハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle();

	/// <summary>
	/// ヒープ番号を進める
	/// </summary>
	void AllocateNextDescriptorNum() { sNextIndexDescriptorHeap_++; }

private:
	/// <summary>
	/// RTV作成
	/// </summary>
	void CreateRenderTargetView();
	/// <summary>
	/// PostEffectTexture作成
	/// </summary>
	void CreateRenderTexture();

private:
	// デバイス関係
	DirectXDevice* dxDevice_ = nullptr;
	// スワップチェーン
	SwapChainManager* swapChainManager_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	uint32_t kDescriptorSizeRTV_ = 0;
	uint32_t size_ = 0;

	int32_t bufferWidth_ = 0;
	int32_t bufferHeight_ = 0;

	const uint32_t kNumDescriptor = 256;

	// RTVのデスク
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_ = {};
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles_;

	float clearColor_[4] = { 0.1f,0.25f,0.5f,1.0f };
	// レンダーテクスチャ
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_;

};
