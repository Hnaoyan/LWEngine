#include "RTVHandler.h"
#include "Utility/DxCreateLib.h"
#include "../../Base/DirectX/DirectXDevice.h"
#include "../../Base/DirectX/SwapChainManager.h"

#include <cassert>

void RTVHandler::StaticInitialize(DirectXDevice* dxDevice, SwapChainManager* swapChainManager)
{
	assert(dxDevice);
	dxDevice_ = dxDevice;
	swapChainManager_ = swapChainManager;
	HRESULT result = S_FALSE;

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.NumDescriptors = 2;

	// Heap生成
	result = dxDevice_->GetDevice()->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvHeap_));
	assert(SUCCEEDED(result));

	// RTV生成
	CreateRenderTargetView();
}

void RTVHandler::CreateRenderTargetView()
{
	HRESULT result = S_FALSE;
	ID3D12Device* device = dxDevice_->GetDevice();
	IDXGISwapChain4* swapChain = swapChainManager_->GetSwapChain();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = swapChain->GetDesc(&swapChainDesc);
	assert(SUCCEEDED(result));

	// RTVヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvHeap_));
	assert(SUCCEEDED(result));

	// RTVデスクの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む

	// 描画枚数
	backBuffer_.resize(swapChainDesc.BufferCount);
	for (int i = 0; i < backBuffer_.size(); ++i) {
		// バッファの取得
		result = swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer_[i]));
		assert(SUCCEEDED(result));

		D3D12_CPU_DESCRIPTOR_HANDLE handle = DxCreateLib::DescriptorLib::GetCPUDescriptorHandle(rtvHeap_.Get(), device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), i);
		
		device->CreateRenderTargetView(backBuffer_[i].Get(), &rtvDesc_, handle);
	}

}
