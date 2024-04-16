#pragma once
#include "../../Utility/Singleton.h"
#include <wrl.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>

class DirectXDevice;

class SwapChainManager : public Singleton<SwapChainManager>
{
public:
	void Initialize(DirectXDevice* dxDevice, int32_t bufferWidth, int32_t bufferHeight);
	void CreateSwapChain();

	void CreateFence();

private:
	// デバイスポインタ
	DirectXDevice* dxDevice_;

	uint32_t bufferWidth_;
	uint32_t bufferHeight_;
	UINT fenceVal_ = 0;

	// スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

};