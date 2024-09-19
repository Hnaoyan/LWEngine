#include "ParticleEmitter.h"
#include "../GPUParticleSystem.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/LwLib/DeltaTime.h"

#include <cassert>

BlendPipeline ParticleEmitter::sPipeline_;
ID3D12GraphicsCommandList* ParticleEmitter::sCommandList = nullptr;

void ParticleEmitter::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	// モデル
	model_ = model;
	// テクスチャ
	texture_ = textureHandle;
	// コマンドリスト
	sCommandList = DirectXCommon::GetInstance()->GetCommandList();
	// 色々なバッファーの作成
	CreateData();
	// GPUでの初期化
	GPUInitialize();

	emitter_.cMap_->count = 24;
	emitter_.cMap_->frequency = 0.5f;
	emitter_.cMap_->frequencyTime = 0.0f;
	emitter_.cMap_->translate = {};
	emitter_.cMap_->radius = 1.0f;
	emitter_.cMap_->emit = 0;
	emitter_.cMap_->emitPattern = 0;

	blendMode_ = BlendMode::kAlpha;

	texture_ = TextureManager::GetInstance()->Load("Resources/Effect/effect.png");
}

void ParticleEmitter::Update()
{
	
	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap() };
	sCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	sCommandList->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.csRootSignature.Get());
	sCommandList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kEmit)].Get());

	BarrierUAV();

#pragma region Emitter更新
	// パーティクルデータ
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), particles_.GetUAVGPU());
	// カウンター
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), freeListIndex_.GetUAVGPU());
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), freeList_.GetUAVGPU());
	// エミッター
	sCommandList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kEmitter), emitter_.cBuffer->GetGPUVirtualAddress());
	// 時間
	sCommandList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kPerTime), perFrame_.cBuffer->GetGPUVirtualAddress());
	sCommandList->Dispatch(1, 1, 1);
#pragma endregion

	BarrierUAV();

#pragma region Particle更新
	sCommandList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kUpdate)].Get());
	// パーティクルデータ
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), particles_.GetUAVGPU());
	// カウンター
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), freeListIndex_.GetUAVGPU());
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), freeList_.GetUAVGPU());
	// エミッター
	sCommandList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kEmitter), emitter_.cBuffer->GetGPUVirtualAddress());
	// 時間
	sCommandList->SetComputeRootConstantBufferView(static_cast<UINT>(Pipeline::GPUParticleRegister::kPerTime), perFrame_.cBuffer->GetGPUVirtualAddress());
	sCommandList->Dispatch(1, 1, 1);
#pragma endregion

	// SRVで使うように設定
	D3D12_RESOURCE_BARRIER barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particles_.cBuffer.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
	sCommandList->ResourceBarrier(1, &barrierUAV);

}

void ParticleEmitter::Draw(ICamera* camera)
{
	// ビューの設定
	perView_.cMap_->viewMatrix = camera->viewMatrix_;
	perView_.cMap_->projectionMatrix = camera->projectionMatrix_;
	perView_.cMap_->billBoardMatrix = Matrix4x4::MakeRotateXYZMatrix(camera->transform_.rotate);

#pragma region 描画
	sPipeline_ = std::get<BlendPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kParticle)]);
	// パイプラインの設定
	sCommandList->SetGraphicsRootSignature(sPipeline_.rootSignature.Get());
	sCommandList->SetPipelineState(sPipeline_.pipelineStates[size_t(blendMode_)].Get());

	//---メッシュの設定---//
	// 頂点バッファの設定
	sCommandList->IASetVertexBuffers(0, 1, &model_->GetMesh()->vbView_);
	// インデックスバッファの設定
	sCommandList->IASetIndexBuffer(&model_->GetMesh()->ibView_);
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		sCommandList, static_cast<UINT>(Pipeline::ParticleRegister::kTexture), texture_);
	// カメラ
	sCommandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(Pipeline::ParticleRegister::kCamera), perView_.cBuffer->GetGPUVirtualAddress());
	// マテリアル
	sCommandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(Pipeline::ParticleRegister::kMaterial), model_->GetMaterial()->buffer_.cBuffer->GetGPUVirtualAddress());
	// 行列
	sCommandList->SetGraphicsRootDescriptorTable(static_cast<UINT>(Pipeline::ParticleRegister::kMatrixs), particles_.GetSRVGPU());
	// 描画処理
	sCommandList->DrawIndexedInstanced(UINT(model_->GetModelData()->indices.size()), GPUParticleSystem::kNumInstanceMax, 0, 0, 0);
#pragma endregion

	// 元のリソースの状態に
	D3D12_RESOURCE_BARRIER barrierUAV = DxCreateLib::ResourceLib::GetResourceBarrier(particles_.cBuffer.Get(),
		D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	sCommandList->ResourceBarrier(1, &barrierUAV);

}

void ParticleEmitter::RefreshData(const EmitterSphere& data)
{
	// エミッターの設定
	emitter_.cMap_->count = data.count;
	emitter_.cMap_->frequency = data.frequency;
	emitter_.cMap_->frequencyTime = data.frequencyTime;
	emitter_.cMap_->translate = data.translate;
	emitter_.cMap_->radius = data.radius;
	emitter_.cMap_->emit = data.emit;
	emitter_.cMap_->emitPattern = data.emitPattern;
}

void ParticleEmitter::UpdataEmitterFlags()
{
	emitter_.cMap_->frequencyTime += kDeltaTime;
	perFrame_.cMap_->time += kDeltaTime;
	if (emitter_.cMap_->frequency <= emitter_.cMap_->frequencyTime) {
		emitter_.cMap_->frequencyTime -= emitter_.cMap_->frequency;
		// フラグ
		emitter_.cMap_->emit = 1;
	}
	else {
		emitter_.cMap_->emit = 0;
	}
}

void ParticleEmitter::CreateData()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// StructuredBuffer
	particles_.CreateBuffer(device, GPUParticleSystem::kNumInstanceMax);
	freeListIndex_.CreateBuffer(device, GPUParticleSystem::kNumInstanceMax);
	freeList_.CreateBuffer(device, GPUParticleSystem::kNumInstanceMax);
	// ConstantBuffer
	perFrame_.CreateConstantBuffer(device);
	perView_.CreateConstantBuffer(device);
	emitter_.CreateConstantBuffer(device);

	// 初期設定
	perFrame_.cMap_->deltaTime = kDeltaTime;
	perFrame_.cMap_->time = 1.0f;
}

void ParticleEmitter::GPUInitialize()
{
	ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSrvHandler()->GetHeap() };
	//// 初期化が後なので
	//D3D12_RESOURCE_BARRIER barrierParticleUAV = DxCreateLib::ResourceLib::TransformToUAV(particles_.cBuffer.Get());
	//sCommandList->ResourceBarrier(1, &barrierParticleUAV);
	//barrierParticleUAV = DxCreateLib::ResourceLib::TransformToUAV(freeListIndex_.cBuffer.Get());
	//sCommandList->ResourceBarrier(1, &barrierParticleUAV);
	//barrierParticleUAV = DxCreateLib::ResourceLib::TransformToUAV(freeList_.cBuffer.Get());
	//sCommandList->ResourceBarrier(1, &barrierParticleUAV);

	sCommandList->SetComputeRootSignature(GraphicsPSO::sParticleGPU_.csRootSignature.Get());
	sCommandList->SetPipelineState(GraphicsPSO::sParticleGPU_.pipelineStates[static_cast<int>(Pipeline::GPUParticlePipeline::kInitialize)].Get());
	sCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// パーティクルデータ
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVParticle), particles_.GetUAVGPU());
	// カウンター
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeListIndex), freeListIndex_.GetUAVGPU());
	sCommandList->SetComputeRootDescriptorTable(static_cast<UINT>(Pipeline::GPUParticleRegister::kUAVFreeList), freeList_.GetUAVGPU());
	// ディスパッチ
	sCommandList->Dispatch(1, 1, 1);
	// バリアの張替え
	BarrierUAV();
}

void ParticleEmitter::BarrierUAV()
{
	// 初期化が後なので
	D3D12_RESOURCE_BARRIER barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(particles_.cBuffer.Get());
	sCommandList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(freeListIndex_.cBuffer.Get());
	sCommandList->ResourceBarrier(1, &barrierParticleUAV);
	barrierParticleUAV = DxCreateLib::ResourceLib::GetUAVBarrier(freeList_.cBuffer.Get());
	sCommandList->ResourceBarrier(1, &barrierParticleUAV);
}

//template class ParticleEmitter<EmitterSphere>;