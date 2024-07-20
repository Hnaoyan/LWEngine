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

struct PerView {
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 billBoardMatrix;
};

template<typename T>
struct ConstantBufferContext
{
	Microsoft::WRL::ComPtr<ID3D12Resource> cBuffer;
	T* cMap_;
};

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
	void Update(ICamera* camera);

	void Draw();


	// この場合板ポリ
	Model* model_ = nullptr;
public:
	// 最大数
	static const uint32_t kNumInstanceMax = 256;
	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> particleResources_;
	Microsoft::WRL::ComPtr<ID3D12Resource> particleUAVResources_;
	ParticleCS* dataMap_ = nullptr;
	ParticleCS* uavDataMap_ = nullptr;

	ConstantBufferContext<PerView> perView_;

	// SRV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandles_;
	uint32_t srvIndex_;
	// UAV情報
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> uavHandles_;
	uint32_t uavIndex_;

	// Unitの数
	uint32_t unitNum_ = 0u;

};
