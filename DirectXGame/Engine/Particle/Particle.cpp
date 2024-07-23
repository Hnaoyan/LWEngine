#include "Particle.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/DeltaTime.h"
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
	particleResources_ = DxCreateLib::ResourceLib::CreateResourceSRV(device, sizeof(ParticleCS) * num);
	//particleResources_->Map(0, nullptr, reinterpret_cast<void**>(&dataMap_));
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
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.NumElements = num;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleCS);
	// リソース
	particleUAVResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(ParticleCS) * num);
	// ハンドル
	uavHandles_.first = SRVHandler::GetSrvHandleCPU();
	uavHandles_.second = SRVHandler::GetSrvHandleGPU();
	uavIndex_ = SRVHandler::AllocateDescriptor();
	device->CreateUnorderedAccessView(particleUAVResources_.Get(), nullptr, &uavDesc, uavHandles_.first);


#pragma region IndexList
	// FreeIndex
	uavDesc.Buffer.StructureByteStride = sizeof(int32_t);
	// リソース
	listIndexUAVResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(int32_t) * num);
	// ハンドル
	listIndexUAVHandles_.first = SRVHandler::GetSrvHandleCPU();
	listIndexUAVHandles_.second = SRVHandler::GetSrvHandleGPU();
	listIndexUAVIndex_ = SRVHandler::AllocateDescriptor();
	device->CreateUnorderedAccessView(listIndexUAVResources_.Get(), nullptr, &uavDesc, listIndexUAVHandles_.first);

	// FreeList
	uavDesc.Buffer.StructureByteStride = sizeof(uint32_t);
	// リソース
	listUAVResources_ = DxCreateLib::ResourceLib::CreateResourceUAV(device, sizeof(uint32_t) * num);
	// ハンドル
	listUAVHandles_.first = SRVHandler::GetSrvHandleCPU();
	listUAVHandles_.second = SRVHandler::GetSrvHandleGPU();
	listUAVIndex_ = SRVHandler::AllocateDescriptor();
	device->CreateUnorderedAccessView(listUAVResources_.Get(), nullptr, &uavDesc, listUAVHandles_.first);
#pragma endregion

}

void Particle::CreateCBuffer()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	perFrame_.CreateConstantBuffer(device);
	perFrame_.Mapping();

	emit_.CreateConstantBuffer(device);
	emit_.Mapping();

	perView_.CreateConstantBuffer(device);
	perView_.Mapping();

}

void Particle::Initialize(Model* model)
{
	model_ = model;

	CreateData();
	CreateCBuffer();

	GPUInitialize();

	perFrame_.cMap_->deltaTime = kDeltaTime;
	perFrame_.cMap_->time = 1.0f;

	emit_.cMap_->count = 24;
	emit_.cMap_->frequency = 0.5f;
	emit_.cMap_->frequencyTime = 0.0f;
	emit_.cMap_->translate = {};
	emit_.cMap_->radius = 1.0f;
	emit_.cMap_->emit = 0;

	texture_ = TextureManager::GetInstance()->Load("Resources/circle.png");

}

void Particle::Update()
{
	emit_.cMap_->frequencyTime += kDeltaTime;
	perFrame_.cMap_->time += kDeltaTime;
	if (emit_.cMap_->frequency <= emit_.cMap_->frequencyTime) {
		emit_.cMap_->frequencyTime -= emit_.cMap_->frequency;
		// フラグ
		emit_.cMap_->emit = 1;
	}
	else {
		emit_.cMap_->emit = 0;
	}


	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap()};
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.rootSignature.Get());

	// 初期化が後なので
	D3D12_RESOURCE_BARRIER barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(particleUAVResources_.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(listIndexUAVResources_.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(listUAVResources_.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);

#pragma region Emitter処理

	cmdList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kEmit)].Get());
	// パーティクルデータ
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), uavHandles_.second);
	// カウンター
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), listIndexUAVHandles_.second);
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), listUAVHandles_.second);
	// エミッター
	cmdList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kEmitter), emit_.cBuffer->GetGPUVirtualAddress());
	// 時間
	cmdList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kPerTime), perFrame_.cBuffer->GetGPUVirtualAddress());
	cmdList->Dispatch(1, 1, 1);

#pragma endregion

	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(particleUAVResources_.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(listIndexUAVResources_.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(listUAVResources_.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);

#pragma region Particle更新
	cmdList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kUpdate)].Get());
	// パーティクルデータ
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), uavHandles_.second);
	// カウンター
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), listIndexUAVHandles_.second);
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), listUAVHandles_.second);
	// 時間
	cmdList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kPerTime), perFrame_.cBuffer->GetGPUVirtualAddress());
	cmdList->Dispatch(1, 1, 1);
#pragma endregion


	///---Structuredに送る処理---///
	// Barrier
	D3D12_RESOURCE_BARRIER barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particleUAVResources_.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	cmdList->ResourceBarrier(1, &barrierUAV);

	D3D12_RESOURCE_BARRIER barrierSRV = DxCreateLib::ResourceLib::GetResourceBarrier(particleResources_.Get(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST);
	cmdList->ResourceBarrier(1, &barrierSRV);

	cmdList->CopyResource(particleResources_.Get(), particleUAVResources_.Get());

	barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particleUAVResources_.Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	cmdList->ResourceBarrier(1, &barrierUAV);
	barrierSRV = DxCreateLib::ResourceLib::GetResourceBarrier(particleResources_.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	cmdList->ResourceBarrier(1, &barrierSRV);
}

void Particle::GPUInitialize()
{
	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap() };
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.rootSignature.Get());
	cmdList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kInitialize)].Get());

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// パーティクルデータ
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), uavHandles_.second);
	// カウンター
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), listIndexUAVHandles_.second);
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), listUAVHandles_.second);

	cmdList->Dispatch(1, 1, 1);

	// Barrier
	D3D12_RESOURCE_BARRIER barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particleUAVResources_.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	cmdList->ResourceBarrier(1, &barrierUAV);

	D3D12_RESOURCE_BARRIER barrierSRV = DxCreateLib::ResourceLib::GetResourceBarrier(particleResources_.Get(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST);
	cmdList->ResourceBarrier(1, &barrierSRV);

	cmdList->CopyResource(particleResources_.Get(), particleUAVResources_.Get());

	barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particleUAVResources_.Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	cmdList->ResourceBarrier(1, &barrierUAV);
	barrierSRV = DxCreateLib::ResourceLib::GetResourceBarrier(particleResources_.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	cmdList->ResourceBarrier(1, &barrierSRV);
}

void Particle::Draw(ICamera* camera) {
	// ビューの設定
	perView_.cMap_->viewMatrix = camera->viewMatrix_;
	perView_.cMap_->projectionMatrix = camera->projectionMatrix_;
	perView_.cMap_->billBoardMatrix = Matrix4x4::MakeRotateXYZMatrix(camera->transform_.rotate);

	sPipeline_ = std::get<GeneralPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kParticle)]);

	ID3D12GraphicsCommandList* cmdList = Model::sCommandList_;

	cmdList->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	cmdList->SetPipelineState(sPipeline_.pipelineState.Get());

	//---メッシュの設定---//
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1,&model_->GetMesh()->vbView_);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&model_->GetMesh()->ibView_);
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		cmdList, static_cast<UINT>(Pipeline::ParticleRegister::kTexture), texture_);
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
