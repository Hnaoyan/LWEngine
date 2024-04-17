#include "RTVHandler.h"
#include "../../Base/DirectX/DirectXDevice.h"

#include <cassert>

void RTVHandler::StaticInitialize(DirectXDevice* dxDevice)
{
	assert(dxDevice);
	dxDevice_ = dxDevice;

	HRESULT result = S_FALSE;

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.NumDescriptors = 1;

	// 生成
	result = dxDevice_->GetDevice()->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvHeap_));
	assert(SUCCEEDED(result));

}
