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
	particles_.CreateBuffer(device, num);
	freeListIndex_.CreateBuffer(device, num);
	freeList_.CreateBuffer(device, num);
}

void Particle::CreateCBuffer()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	perFrame_.CreateConstantBuffer(device);
	//perFrame_.Mapping();

	emit_.CreateConstantBuffer(device);
	//emit_.Mapping();

	perView_.CreateConstantBuffer(device);
	//perView_.Mapping();

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
	cmdList->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.csRootSignature.Get());

	// 初期化が後なので
	D3D12_RESOURCE_BARRIER barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(particles_.cBuffer.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(freeListIndex_.cBuffer.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(freeList_.cBuffer.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);

#pragma region Emitter処理

	cmdList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kEmit)].Get());
	// パーティクルデータ
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), particles_.GetUAVGPU());
	// カウンター
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), freeListIndex_.GetUAVGPU());
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), freeList_.GetUAVGPU());
	// エミッター
	cmdList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kEmitter), emit_.cBuffer->GetGPUVirtualAddress());
	// 時間
	cmdList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kPerTime), perFrame_.cBuffer->GetGPUVirtualAddress());
	cmdList->Dispatch(1, 1, 1);

#pragma endregion

	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(particles_.cBuffer.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(freeListIndex_.cBuffer.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(freeList_.cBuffer.Get());
	cmdList->ResourceBarrier(1, &barrierParticleUAV);

#pragma region Particle更新
	cmdList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kUpdate)].Get());
	// パーティクルデータ
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), particles_.GetUAVGPU());
	// カウンター
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), freeListIndex_.GetUAVGPU());
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), freeList_.GetUAVGPU());
	// 時間
	cmdList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kPerTime), perFrame_.cBuffer->GetGPUVirtualAddress());
	cmdList->Dispatch(1, 1, 1);
#pragma endregion

	D3D12_RESOURCE_BARRIER barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particles_.cBuffer.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
	cmdList->ResourceBarrier(1, &barrierUAV);

}

void Particle::GPUInitialize()
{
	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap() };
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.csRootSignature.Get());
	cmdList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kInitialize)].Get());

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// パーティクルデータ
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), particles_.GetUAVGPU());
	// カウンター
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), freeListIndex_.GetUAVGPU());
	cmdList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), freeList_.GetUAVGPU());

	cmdList->Dispatch(1, 1, 1);
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
	cmdList->SetGraphicsRootConstantBufferView(static_cast<UINT>(Pipeline::ParticleRegister::kMaterial), model_->GetMaterial()->buffer_.cBuffer->GetGPUVirtualAddress());
	// 行列
	cmdList->SetGraphicsRootDescriptorTable(static_cast<UINT>(Pipeline::ParticleRegister::kMatrixs), particles_.GetSRVGPU());
	
	//cmdList->DrawInstanced(6, 1024, 0, 0);
	cmdList->DrawIndexedInstanced(UINT(model_->GetModelData()->indices.size()), 1024, 0, 0, 0);
	//cmdList->SetGraphicsRootConstantBufferView();
	D3D12_RESOURCE_BARRIER barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particles_.cBuffer.Get(),
		D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	cmdList->ResourceBarrier(1, &barrierUAV);

}
