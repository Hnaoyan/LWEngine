#include "SRVHandler.h"
#include "../../Base/DirectX/DirectXDevice.h"

#include <cassert>

uint32_t SRVHandler::kDescriptorSizeSRV_ = 0u;
uint32_t SRVHandler::sNowDescriptorNum_ = 2;
uint32_t SRVHandler::sNextDescriptorNum_ = 2;
DirectXDevice* SRVHandler::dxDevice_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVHandler::srvHeap_;


void SRVHandler::StaticInitialize(DirectXDevice* dxDevice)
{
	HRESULT result = S_FALSE;
	dxDevice_ = dxDevice;

	kDescriptorSizeSRV_ = dxDevice_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_DESCRIPTOR_HEAP_DESC srvDescriptorHeapDesc{};
	srvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescriptorHeapDesc.NumDescriptors = kDescpritorSize;
	srvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	result = dxDevice_->GetDevice()->CreateDescriptorHeap(&srvDescriptorHeapDesc, IID_PPV_ARGS(&srvHeap_));

	assert(SUCCEEDED(result));

}

D3D12_CPU_DESCRIPTOR_HANDLE SRVHandler::GetSrvHandleCPU()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += size_t(kDescriptorSizeSRV_ * sNextDescriptorNum_);
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRVHandler::GetSrvHandleGPU()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = srvHeap_->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += size_t(kDescriptorSizeSRV_ * sNextDescriptorNum_);
	return handle;
}
