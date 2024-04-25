#pragma once
#include "../../Utility/Singleton.h"
#include "d3d12.h"
#include <wrl.h>
#include <stdint.h>

class DirectXDevice;

class SRVHandler : public Singleton<SRVHandler>
{
public:
	// 最大カウント
	static const int max = 256;
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dxDevice"></param>
	void StaticInitialize(DirectXDevice* dxDevice);

	void AddPtr(uint32_t index, D3D12_DESCRIPTOR_HEAP_TYPE type);

public: // アクセッサ
	uint32_t const GetIndex() { return index_; }
	uint32_t const GetSizeSRV() { return kDescriptorSizeSRV_; }
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t textureHandle) {
		return handleGPU[textureHandle];
	}
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t textureHandle) {
		return handleCPU[textureHandle];
	}
	void SetGPUHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index) {
		handleGPU[index] = GetSrvHandleGPU(descriptorheap, descriptorSize, index + 1);
	}
	void SetCPUHandle(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index) {
		handleCPU[index] = GetSrvHandleCPU(descriptorheap, descriptorSize, index + 1);
	}
	D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandleCPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// SRVヒープの取得
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap_.Get(); }
private:
	// デバイスクラス
	DirectXDevice* dxDevice_;
	// DescriptorHeap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	uint32_t index_ = 0;
	uint32_t kDescriptorSizeSRV_ = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU[max] = {};
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU[max] = {};
};

