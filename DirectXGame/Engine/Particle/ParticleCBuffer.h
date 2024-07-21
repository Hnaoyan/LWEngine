#pragma once
#include <memory>

#include "Engine/Math/MathLib.h"
#include "Engine/3D/Drawer/Model.h"

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
struct ConstantBufferMapContext
{
	Microsoft::WRL::ComPtr<ID3D12Resource> cBuffer;
	T* cMap_;
};

struct HeapAllocationData
{
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> handles;
	uint32_t index;
};
