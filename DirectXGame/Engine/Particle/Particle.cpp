#include "Particle.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"
#include <cassert>

GeneralPipeline Particle::sPipeline_;

void Particle::CreateData()
{
	int num = 1024;
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = num;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleCS);
	// リソース
	particleResources_ = DxCreateLib::ResourceLib::CreateBufferResource(device, sizeof(ParticleCS) * num);
	particleResources_->Map(0, nullptr, reinterpret_cast<void**>(&dataMap_));
	// SRVの設定
	srvHandles_.first = SRVHandler::GetSrvHandleCPU();
	srvHandles_.second = SRVHandler::GetSrvHandleGPU();
	srvIndex_ = SRVHandler::AllocateDescriptor();
	device->CreateShaderResourceView(particleResources_.Get(), &instancingSrvDesc, srvHandles_.first);

	// UAV
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = num;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleCS);
	// リソース
	particleUAVResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(ParticleCS) * num);
	// ハンドル
	uavHandles_.first = SRVHandler::GetSrvHandleCPU();
	uavHandles_.second = SRVHandler::GetSrvHandleGPU();
	uavIndex_ = SRVHandler::AllocateDescriptor();
	device->CreateUnorderedAccessView(particleUAVResources_.Get(), nullptr, &uavDesc, uavHandles_.first);
	//particleUAVResources_->Map(0, nullptr, reinterpret_cast<void**>(&uavDataMap_));

}

void Particle::CreateCBuffer()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	perView_.cBuffer = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(PerView) + 0xff) & ~0xff);
	HRESULT result = S_FALSE;
	result = perView_.cBuffer->Map(0, nullptr, (void**)&perView_.cMap_);
	assert(SUCCEEDED(result));
}

void Particle::Initialize(Model* model)
{
	model_ = model;

	CreateData();
	CreateCBuffer();
}

void Particle::Update(ICamera* camera)
{
	perView_.cMap_->viewMatrix = camera->viewMatrix_;
	perView_.cMap_->projectionMatrix = camera->projectionMatrix_;
	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(camera->transform_.scale, camera->transform_.rotate, camera->transform_.translate);

	perView_.cMap_->billBoardMatrix = Matrix4x4::MakeBillBoardMatrix(cameraMatrix);

	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap()};
	Model::sCommandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	Model::sCommandList_->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.rootSignature.Get());
	Model::sCommandList_->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineState.Get());
	Model::sCommandList_->SetComputeRootDescriptorTable(0, uavHandles_.second);
	Model::sCommandList_->Dispatch(1, 1, 1);
	//Model::sCommandList_->Dispatch(UINT(model_->GetModelData()->vertices.size() + 1023) / 1024, 1, 1);

	//for (int i = 0; i < 1024; ++i) {
	//	dataMap_[i] = uavDataMap_[i];
	//}
	//dataMap_->worldMatrix
}

void Particle::Draw() {

	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kParticle)]);

	ID3D12GraphicsCommandList* cmdList = Model::sCommandList_;

	cmdList->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	cmdList->SetPipelineState(sPipeline_.pipelineState.Get());
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		cmdList, static_cast<UINT>(Pipeline::ParticleRegister::kTexture), model_->GetModelData()->material.textureHandle);
	// カメラ
	cmdList->SetGraphicsRootConstantBufferView(static_cast<UINT>(Pipeline::ParticleRegister::kCamera), perView_.cBuffer->GetGPUVirtualAddress());
	// マテリアル
	cmdList->SetGraphicsRootConstantBufferView(static_cast<UINT>(Pipeline::ParticleRegister::kMaterial), model_->GetMaterial()->materialBuff_->GetGPUVirtualAddress());
	// 行列
	cmdList->SetGraphicsRootDescriptorTable(static_cast<UINT>(Pipeline::ParticleRegister::kMatrixs), srvHandles_.second);
	
	//cmdList->DrawInstanced(6, 1024, 0, 0);
	cmdList->DrawIndexedInstanced(UINT(model_->GetModelData()->indices.size()), 1024, 0, 0, 0);
	//cmdList->SetGraphicsRootConstantBufferView();

}
