#include "DSVHandler.h"
#include "../../Base/Utility/DxCreateLib.h"
#include "../../Base/DirectX/DirectXDevice.h"
#include "../../WindowAPI/WindowAPI.h"

#include <cassert>

using namespace Microsoft::WRL;

void DSVHandler::StaticInitialize(DirectXDevice* dxDevice)
{
	assert(dxDevice);
	dxDevice_ = dxDevice;
	bufferWidth_ = WindowAPI::kWindowWidth;
	bufferHeight_ = WindowAPI::kWindowHeight;
	CreateDepthBuffer();
}

void DSVHandler::ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList)
{
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	// 指定した深度で画面全体をクリアする
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DSVHandler::CreateDepthBuffer()
{
	ID3D12Device* device = dxDevice_->GetDevice();
	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc = DxCreateLib::ResourceLib::CreateResourceDesc(
		DXGI_FORMAT_D32_FLOAT, D3D12_RESOURCE_DIMENSION_TEXTURE2D, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, bufferWidth_, bufferHeight_);
	resourceDesc.MipLevels = 0;

	// Heapの設定
	D3D12_HEAP_PROPERTIES heapProp = DxCreateLib::HeapLib::CreateHeapProps(D3D12_HEAP_TYPE_DEFAULT);

	// 深度値
	D3D12_CLEAR_VALUE depthClearValue = DxCreateLib::DSVLib::CreateClearValue(DXGI_FORMAT_D32_FLOAT, 1.0f);

	// Resourceの生成
	ComPtr<ID3D12Resource> resource;
	HRESULT result = S_FALSE;
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	// DepthStencilResourceの設定
	depthStencilResource_ = resource;
	// DSV用のヒープ
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = DxCreateLib::HeapLib::CreateDescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	result = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	assert(SUCCEEDED(result));

	// DSVDescの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc{};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	// DSVHeapの先頭にDSVをつくる
	device->CreateDepthStencilView(depthStencilResource_.Get(), &depthStencilDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());

}
