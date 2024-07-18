#include "Particle.h"
#include "Engine/Base/DirectXCommon.h"
#include <cassert>

void Particle::CreateCSResource()
{
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	
	particleResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(ParticleCS) * 1024);

}

void Particle::CreateData()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstanceMax;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleGPU);
	// リソース
	particleResources_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(ParticleGPU) * kNumInstanceMax);
	// UAV
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = (UINT)model_->GetModelData()->vertices.size();
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(VertexData);
}

void Particle::Initialize(Model* model)
{
	model_ = model;
}
