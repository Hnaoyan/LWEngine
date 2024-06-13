#include "RTVHandler.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "../../Base/DirectX/DirectXDevice.h"
#include "../../Base/DirectX/SwapChainManager.h"
#include "SRVHandler.h"

#include <cassert>

uint32_t RTVHandler::sNextIndexDescriptorHeap_ = 0u;

void RTVHandler::StaticInitialize(DirectXDevice* dxDevice, SwapChainManager* swapChainManager)
{
	assert(dxDevice);
	dxDevice_ = dxDevice;
	swapChainManager_ = swapChainManager;
	// 縦横のサイズ
	bufferWidth_ = swapChainManager_->GetBufferWidth();
	bufferHeight_ = swapChainManager_->GetBufferHeight();
	// RTVのサイズ
	kDescriptorSizeRTV_ = dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	HRESULT result = S_FALSE;

	// RTVヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDescHeapDesc.NumDescriptors = kNumDescriptor;
	result = dxDevice_->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvHeap_));
	assert(SUCCEEDED(result));

	// RTV生成
	CreateRenderTargetView();
	// RenderTexture生成
	CreateRenderTexture();
}

void RTVHandler::ClearRenderTarget(ID3D12GraphicsCommandList* cmdList)
{
	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChainManager_->GetSwapChain()->GetCurrentBackBufferIndex();

	// 描画先のRTVを設定する
	// ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxCreateLib::DescriptorLib::GetCPUDescriptorHandle(rtvHeap_.Get(), dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), backBufferIndex);

	// 指定した色で画面全体をクリアする
	cmdList->ClearRenderTargetView(rtvHandle, clearColor_, 0, nullptr);
}


void RTVHandler::CreateRenderTargetView()
{
	HRESULT result = S_FALSE;
	ID3D12Device* device = dxDevice_->GetDevice();
	IDXGISwapChain4* swapChain = swapChainManager_->GetSwapChain();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = swapChain->GetDesc(&swapChainDesc);
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

		// ハンドルの設定
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUDescriptorHandle();
		// 進める
		AllocateNextDescriptorNum();

		device->CreateRenderTargetView(backBuffer_[i].Get(), &rtvDesc_, handle);
	}

}

void RTVHandler::CreateRenderTexture()
{

	SRVHandler* srv = SRVHandler::GetInstance();
	ID3D12Device* device = dxDevice_->GetDevice();

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUDescriptorHandle();
	AllocateNextDescriptorNum();

	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 2Dテクスチャとして書き込む

	renderTextureResource_ = DxCreateLib::RenderTextureLib::CreateRenderTextureResource(device, bufferWidth_, bufferHeight_, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, {1,1,1,1});
	device->CreateRenderTargetView(renderTextureResource_.Get(), &rtvDesc_, handle);
	//SRVの作成
		//D3D12_CPU_DESCRIPTOR_HANDLE handleSrv = DescriptorManager::GetCPUDescriptorHandle(heap_.Get(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 3);
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srv->GetSrvHandleCPU();
	srv->AllocateNextDescriptorNum();
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = DescriptorManager::GetCPUDescriptorHandle(srv->GetHeap(), device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 2);
	device->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, srvHandle);


}

D3D12_CPU_DESCRIPTOR_HANDLE RTVHandler::GetCPUDescriptorHandle()
{
	uint32_t size = dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = rtvHeap_->GetCPUDescriptorHandleForHeapStart();

	handleCPU.ptr += (size * sNextIndexDescriptorHeap_);

	return handleCPU;
}
