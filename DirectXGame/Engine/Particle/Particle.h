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

	RWStructuredBufferContext<ParticleCS> particles_;
	RWStructuredBufferContext<int32_t> freeListIndex_;
	RWStructuredBufferContext<uint32_t> freeList_;

	// Viewのリソース
	ConstantBufferMapContext<PerView> perView_;
	ConstantBufferMapContext<EmitterSphere> emit_;
	ConstantBufferMapContext<PerFrame> perFrame_;

	// Unitの数
	uint32_t unitNum_ = 0u;

	uint32_t texture_ = 0u;
};
