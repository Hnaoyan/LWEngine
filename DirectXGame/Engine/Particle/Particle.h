#pragma once
#include <memory>

#include "ParticleCBuffer.h"
#include "Engine/Base/Utility/DxCreateLib.h"
#include "Engine/3D/Drawer/Model.h"

class ICamera;

class Particle
{
private:
	// パイプライン
	static GeneralPipeline sPipeline_;
public:
	///
	/// UAVとSRVの作成
	/// 
	void CreateData();
	void CreateCBuffer();
	void Initialize(Model* model);
	void Update();
	void GPUInitialize();
	void Draw(ICamera* camera);


	// この場合板ポリ
	Model* model_ = nullptr;
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 256;
	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> particleResources_;
	Microsoft::WRL::ComPtr<ID3D12Resource> particleUAVResources_;
	Microsoft::WRL::ComPtr<ID3D12Resource> listIndexUAVResources_;
	Microsoft::WRL::ComPtr<ID3D12Resource> listUAVResources_;

	// Viewのリソース
	ConstantBufferMapContext<PerView> perView_;
	ConstantBufferMapContext<EmitterSphere> emit_;
	ConstantBufferMapContext<PerFrame> perFrame_;

	// SRV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandles_;
	uint32_t srvIndex_;

	HeapAllocationData srvAllocater_;

	// UAV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> uavHandles_;
	uint32_t uavIndex_;

	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> listIndexUAVHandles_;
	uint32_t listIndexUAVIndex_;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> listUAVHandles_;
	uint32_t listUAVIndex_;
	// Unitの数
	uint32_t unitNum_ = 0u;


	uint32_t texture_ = 0u;
};
