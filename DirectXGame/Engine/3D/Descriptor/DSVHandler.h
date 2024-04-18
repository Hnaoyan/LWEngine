#pragma once
#include "../../Utility/Singleton.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class DirectXDevice;

class DSVHandler : public Singleton<DSVHandler>
{
public:

	void StaticInitialize(DirectXDevice* dxDevice);

	void ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList);

	ID3D12DescriptorHeap* GetHeap() { return dsvHeap_.Get(); }

private:
	void CreateDepthBuffer();

private:
	// デバイス管理クラス
	DirectXDevice* dxDevice_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	// インフォ
	uint32_t kDescriptorSize_ = 0;
	uint32_t size_ = 0;
	uint32_t bufferWidth_ = 0;
	uint32_t bufferHeight_ = 0;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};

};