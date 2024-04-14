#pragma once
#include "../../Utility/Singleton.h"
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <vector>

class DirectXCommon;

class RTV : public Singleton<RTV>
{
public:
	void StaticInitialize(DirectXCommon* dxCommon, int32_t bufferWidth, int32_t bufferHeight);

private:
	DirectXCommon* dxCommon_ = nullptr;
	// デバイス
	ID3D12Device* device_ = nullptr;
	// ヒープ自体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	uint32_t kDescriptorSize_;
	uint32_t size_;

	uint32_t bufferWidth_;
	uint32_t bufferHeight_;

	// RTVのデスク
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
	float clearColor_[4] = { 0.1f,0.25f,0.5f,1.0f };

};
