#include "SwapChainManager.h"
#include "DirectXCommand.h"
#include "DirectXDevice.h"
#include "../../WindowAPI/WindowAPI.h"
#include <cassert>

void SwapChainManager::Initialize(DirectXDevice* dxDevice, int32_t bufferWidth, int32_t bufferHeight)
{
	assert(dxDevice);
	dxDevice_ = dxDevice;
	bufferWidth_ = bufferWidth;
	bufferHeight_ = bufferHeight;

	CreateSwapChain();
	CreateFence();

}

void SwapChainManager::CreateSwapChain()
{
	HRESULT result = S_FALSE;
	// SwapChainDesc作成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// 画面サイズ
	swapChainDesc.Width = bufferWidth_;
	swapChainDesc.Height = bufferHeight_;
	// 色の形式
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// マルチサンプルの有無
	swapChainDesc.SampleDesc.Count = 1;
	// 描画ターゲットとして
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;	// ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタにうつしたら破棄

	// 設定を渡すため
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;

	// SwapChain1で生成
	result = dxDevice_->GetDXGIFactory()->CreateSwapChainForHwnd(
		DirectXCommand::GetInstance()->commandQueue_.Get(),
		WindowAPI::GetInstance()->GetHwnd(),
		&swapChainDesc,
		nullptr, nullptr,
		&swapChain1);
	assert(SUCCEEDED(result));

	// SwapChain1を変換
	result = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(result));

}

void SwapChainManager::CreateFence()
{
	HRESULT result = S_FALSE;

	// Fenceの生成
	result = dxDevice_->GetDevice()->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));

}
