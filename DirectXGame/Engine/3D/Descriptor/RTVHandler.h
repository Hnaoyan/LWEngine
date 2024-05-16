#pragma once
#include "../../Utility/Singleton.h"
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <vector>

class DirectXDevice;
class SwapChainManager;

class RTVHandler : public Singleton<RTVHandler>
{
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


public: // アクセッサ
	D3D12_RENDER_TARGET_VIEW_DESC GetDesc() { return rtvDesc_; }
	ID3D12Resource* GetBackBuffer(UINT index) { return backBuffer_[index].Get(); }
	size_t GetBackBufferCount() const { return backBuffer_.size(); }

	/// <summary>
	/// RTVヒープの取得
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetRtvHeap() { return rtvHeap_.Get(); }

private:
	/// <summary>
	/// RTV作成
	/// </summary>
	void CreateRenderTargetView();

private:
	DirectXDevice* dxDevice_ = nullptr;
	SwapChainManager* swapChainManager_ = nullptr;
	// デバイス
	//ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	uint32_t kDescriptorSizeRTV_ = 0;
	uint32_t size_ = 0;

	// RTVのデスク
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_ = {};
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles_;

	float clearColor_[4] = { 0.1f,0.25f,0.5f,1.0f };

};