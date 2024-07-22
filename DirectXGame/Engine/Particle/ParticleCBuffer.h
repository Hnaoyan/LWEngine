#pragma once
#include <memory>
#include <cassert>
#include <type_traits>
#include <concepts>

#include "Engine/Math/MathLib.h"
#include "Engine/3D/Drawer/Model.h"

struct EmitterSphere {
	Vector3 translate;	// 位置
	// 射出関係
	float radius;		// 半径
	uint32_t count;		// 数
	float frequency;	// 間隔
	float frequencyTime; // 間隔時間
	uint32_t emit;	// 許可フラグ
};

struct PerFrame {
	// 起動してからの時間
	float time;
	float deltaTime;
};

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

enum class BufferType {
	kConstantBuffer,
	kStructuredBuffer,
	kRWStructuredBuffer,
};

struct HeapAllocationData
{
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> handles;
	uint32_t index;
};

template<typename T>
concept IsStruct = std::is_class_v<T>;

template<IsStruct T>
struct ConstantBufferMapContext
{
	// BufferResource
	Microsoft::WRL::ComPtr<ID3D12Resource> cBuffer;
	// Map
	T* cMap_;


	void CreateConstantBuffer(ID3D12Device* device) {
		// 作成
		cBuffer = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(T) + 0xff) & ~0xff);
	}
	void Mapping() {
		// マッピング
		HRESULT result = S_FALSE;
		result = cBuffer->Map(0, nullptr, (void**)&cMap_);
		assert(SUCCEEDED(result));
	}
	//void CreateStructuredBuffer();

};

