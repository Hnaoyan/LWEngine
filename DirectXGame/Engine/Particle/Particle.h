#pragma once
#include <memory>

#include "Engine/Base/Utility/DxCreateLib.h"
#include "Engine/3D/Drawer/Model.h"

class ICamera;

struct ParticleCS {
	Vector3 translate;
	Vector3 scale;
	float lifetime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

struct ParticleGPU {
	Matrix4x4 worldMatrix;
	Vector4 color;
};

class Particle
{
public:
	//void CreateUAV();
	//void CreateSRV();
	void CreateData();
	void Initialize(Model* model);
	void Update(ICamera* camera);
	// この場合板ポリ
	Model* model_ = nullptr;
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 256;
	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> particleResources_;
	Microsoft::WRL::ComPtr<ID3D12Resource> particleUAVResources_;
	ParticleGPU* dataMap_ = nullptr;
	Particle* uavDataMap_ = nullptr;

	// SRV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandles_;
	uint32_t srvIndex_;
	// UAV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> uavHandles_;
	uint32_t uavIndex_;

	// Unitの数
	uint32_t unitNum_ = 0u;

};
