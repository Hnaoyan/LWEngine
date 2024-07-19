#include "Particle.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"
#include <cassert>

GeneralPipeline Particle::sPipeline_;

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
	particleResources_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(ParticleGPU) * 1024);
	particleResources_->Map(0, nullptr, reinterpret_cast<void**>(&dataMap_));
	// SRVの設定
	srvHandles_.first = SRVHandler::GetSrvHandleCPU();
	srvHandles_.second = SRVHandler::GetSrvHandleGPU();
	srvIndex_ = SRVHandler::AllocateDescriptor();

	// UAV
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = (UINT)model_->GetModelData()->vertices.size();
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(VertexData);
	// リソース
	particleUAVResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(ParticleCS) * 1024);
	//particleUAVResources_->Map(0, nullptr, reinterpret_cast<void**>(&uavDataMap_));
	// ハンドル
	uavHandles_.first = SRVHandler::GetSrvHandleCPU();
	uavHandles_.second = SRVHandler::GetSrvHandleGPU();
	uavIndex_ = SRVHandler::AllocateDescriptor();
	device->CreateUnorderedAccessView(particleUAVResources_.Get(), nullptr, &uavDesc, uavHandles_.first);

}

void Particle::Initialize(Model* model)
{
	model_ = model;

	CreateData();
}

void Particle::Update(ICamera* camera)
{

	camera;
	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap()};
	Model::sCommandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	Model::sCommandList_->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.rootSignature.Get());
	Model::sCommandList_->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineState.Get());
	Model::sCommandList_->SetComputeRootDescriptorTable(0, uavHandles_.second);
	Model::sCommandList_->Dispatch(UINT(model_->GetModelData()->vertices.size() + 1023) / 1024, 1, 1);
}

void Particle::Draw(const ModelDrawDesc& desc) {

	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kParticle)]);

	ID3D12GraphicsCommandList* cmdList = Model::sCommandList_;

	cmdList->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	cmdList->SetPipelineState(sPipeline_.pipelineState.Get());

	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		cmdList, static_cast<UINT>(Pipeline::ParticleRegister::kTexture), model_->GetModelData()->material.textureHandle);

	//cmdList->SetGraphicsRootConstantBufferView();

}
