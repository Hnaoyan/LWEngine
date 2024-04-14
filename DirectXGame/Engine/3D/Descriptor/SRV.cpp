#include "SRV.h"
#include "../../Base/DirectX/DirectXDevice.h"

#include <cassert>

void SRV::StaticInitialize(DirectXDevice* dxDevice)
{
	HRESULT result = S_FALSE;
	dxDevice_ = dxDevice;

	D3D12_DESCRIPTOR_HEAP_DESC srvDescriptorHeapDesc{};
	srvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescriptorHeapDesc.NumDescriptors = max;
	srvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = dxDevice_->GetDevice()->CreateDescriptorHeap(&srvDescriptorHeapDesc, IID_PPV_ARGS(&heap_));

	assert(SUCCEEDED(result));

	descriptorSizeSRV_ = dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	index_ = 2;

}

void SRV::AddPtr(uint32_t index, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	handleCPU[index].ptr += dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(type);
	handleGPU[index].ptr += dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(type);
}

D3D12_CPU_DESCRIPTOR_HANDLE SRV::GetSrvHandleCPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorheap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (descriptorSize * index);
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRV::GetSrvHandleGPU(ID3D12DescriptorHeap* descriptorheap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = descriptorheap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (descriptorSize * index);
	return handle;
}
