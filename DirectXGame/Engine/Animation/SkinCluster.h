#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <span>
#include <array>
#include <vector>
#include "../Math/MathLib.h"

class Skeleton;
class ModelData;

struct VertexWeightData
{
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

const uint32_t kNumMaxInfluence = 4;

struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;	// 位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;	// 法線用
};

class SkinCluster
{
public:
	static SkinCluster Create(ID3D12Device* device, const Skeleton& skeleton, const ModelData& modelData);

public:

	std::vector<Matrix4x4> inverseBindPoseMatrices;
	// Influence
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	// MatrixPalette
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
	uint32_t srvHandleIndex;
};
