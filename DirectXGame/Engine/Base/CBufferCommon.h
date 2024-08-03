#pragma once
#include <memory>
#include <cassert>
#include <type_traits>
#include <concepts>

#include "Engine/Math/MathLib.h"
#include "Engine/3D/Descriptor/SRVHandler.h"
#include "Engine/Base/Utility/DxCreateLib.h"

enum class BufferType : uint32_t{
	kConstantBuffer,
	kStructuredBuffer,
	kRWStructuredBuffer,
};

struct HeapAllocationData
{
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> handles;
	uint32_t index = 0u;
};

template<typename T>
concept IsStruct = std::is_class_v<T>;

template<typename T>
struct ConstantBufferMapContext
{
public:
	// BufferResource
	Microsoft::WRL::ComPtr<ID3D12Resource> cBuffer;
	// Map
	T* cMap_ = nullptr;

	/// <summary>
	/// バッファー作成
	/// </summary>
	/// <param name="device"></param>
	void CreateConstantBuffer(ID3D12Device* device) {
		cBuffer = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(T) + 0xff) & ~0xff);
		Mapping();
	}
private:
	/// <summary>
	/// マッピング
	/// </summary>
	void Mapping() {
		HRESULT result = S_FALSE;
		result = cBuffer->Map(0, nullptr, (void**)&cMap_);
		assert(SUCCEEDED(result));
	}

};

template<typename T>
struct RWStructuredBufferContext
{
	// Buffer
	Microsoft::WRL::ComPtr<ID3D12Resource> cBuffer;

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPU() const { return srvHeapData.handles.first; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPU() const { return srvHeapData.handles.second; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPU() const { return uavHeapData.handles.first; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPU() const { return uavHeapData.handles.second; }

	// SRV用
	HeapAllocationData srvHeapData{};
	// UAV用
	HeapAllocationData uavHeapData{};
	/// <summary>
	/// バッファーデータ作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="maxNum"></param>
	void CreateBuffer(ID3D12Device* device, int32_t maxNum);

};

template<typename T>
inline void RWStructuredBufferContext<T>::CreateBuffer(ID3D12Device* device, int32_t maxNum)
{
#pragma region SRV
	// SRVDesc
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = maxNum;
	srvDesc.Buffer.StructureByteStride = sizeof(T);
	// Heap
	srvHeapData.handles.first = SRVHandler::GetSrvHandleCPU();
	srvHeapData.handles.second = SRVHandler::GetSrvHandleGPU();
	srvHeapData.index = SRVHandler::AllocateDescriptor();
#pragma endregion

#pragma region UAV
	// UAVDesc
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.NumElements = maxNum;
	uavDesc.Buffer.StructureByteStride = sizeof(T);
	// Heap
	uavHeapData.handles.first = SRVHandler::GetSrvHandleCPU();
	uavHeapData.handles.second = SRVHandler::GetSrvHandleGPU();
	uavHeapData.index = SRVHandler::AllocateDescriptor();
#pragma endregion
	// Resource作成
	cBuffer = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(T) * maxNum);
	// SRV
	device->CreateShaderResourceView(cBuffer.Get(), &srvDesc, srvHeapData.handles.first);
	// UAV
	device->CreateUnorderedAccessView(cBuffer.Get(), nullptr, &uavDesc, uavHeapData.handles.first);
}
