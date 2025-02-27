#pragma once
#include "../../Utility/Singleton.h"
#include <wrl.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>

class DirectXDevice;

/// <summary>
/// スワップチェーン管理クラス
/// </summary>
class SwapChainManager : public Singleton<SwapChainManager>
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxDevice"></param>
	/// <param name="bufferWidth"></param>
	/// <param name="bufferHeight"></param>
	void Initialize(DirectXDevice* dxDevice, int32_t bufferWidth, int32_t bufferHeight);

public: // アクセッサ
	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }
	ID3D12Fence* GetFence() { return fence_.Get(); }
	uint32_t GetBufferWidth() { return bufferWidth_; }
	uint32_t GetBufferHeight() { return bufferHeight_; }
private: // 生成関数
	/// <summary>
	/// SwapChain生成
	/// </summary>
	void CreateSwapChain();
	/// <summary>
	/// Fence生成
	/// </summary>
	void CreateFence();

private:
	// デバイスポインタ
	DirectXDevice* dxDevice_ = nullptr;

	uint32_t bufferWidth_ = 0;
	uint32_t bufferHeight_ = 0;
	UINT fenceVal_ = 0;

	// スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

};