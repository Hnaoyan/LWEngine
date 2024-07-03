#include "PostEffectRender.h"
#include "../3D/Descriptor/SRVHandler.h"
#include "../Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"
#include <cassert>

Pipeline::PostEffectType PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;
uint32_t PostEffectRender::sDissolveTexture = 0u;

void PostEffectRender::StaticInitialize()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// テクスチャ
	sDissolveTexture = TextureManager::Load("Resources/Dissolve/noise1.png");

	// リソース作成
	vignetteCBuffer_ = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(CBufferDataVignette) + 0xff) & ~0xff);
	blurCBuffer_ = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(CBufferDataBlur) + 0xff) & ~0xff);
	dissolveCBuffer_ = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(CBufferDataDissolve) + 0xff) & ~0xff);
	noiseCBuffer_ = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(CBufferDataNoise) + 0xff) & ~0xff);

#pragma region マッピング
	HRESULT result = S_FALSE;
	result = vignetteCBuffer_->Map(0, nullptr, (void**)&vignetteMap_);
	assert(SUCCEEDED(result));
	result = blurCBuffer_->Map(0, nullptr, (void**)&blurMap_);
	assert(SUCCEEDED(result));
	result = dissolveCBuffer_->Map(0, nullptr, (void**)&dissolveMap_);
	assert(SUCCEEDED(result));
	result = noiseCBuffer_->Map(0, nullptr, (void**)&noiseMap_);
	assert(SUCCEEDED(result));
#pragma endregion

	vignetteMap_->scale = 16.0f;
	vignetteMap_->powValue = 0.8f;
	vignetteMap_->color = { 1.0f,0.0f,0.0f };

	blurMap_->centerPoint = { 0.5f,0.5f };
	blurMap_->samplesNum = 5;
	blurMap_->blurWidth = 0.01f;

	dissolveMap_->color = {};
	dissolveMap_->threshold = 0;

	noiseMap_->time = 0;
	noiseMap_->enableScreen = 0;

}

void PostEffectRender::Update(const PostEffectDesc& desc)
{
	vignetteMap_->color = desc.vignette.color;
	vignetteMap_->powValue = desc.vignette.powValue;
	vignetteMap_->scale = desc.vignette.scale;

	blurMap_->blurWidth = desc.blur.blurWidth;
	blurMap_->centerPoint = desc.blur.centerPoint;
	blurMap_->samplesNum = desc.blur.samplesNum;

	dissolveMap_->color = desc.dissolve.color;
	dissolveMap_->threshold = desc.dissolve.threshold;

	// ノイズでの処理時のみ
	if (sPostEffect != Pipeline::PostEffectType::kNoise) {
		noiseMap_->time = 0;
		return;
	}
	noiseMap_->time++;
	noiseMap_->enableScreen = desc.noise.enableScreen;
}

void PostEffectRender::Draw(ID3D12GraphicsCommandList* cmdList)
{
	commandList_ = cmdList;

	PostEffectPipeline pipeline = std::get<PostEffectPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kPostEffect)]);
	// PSO
	commandList_->SetPipelineState(pipeline.pipelineStates[size_t(sPostEffect)].Get());
	commandList_->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	// SRVヒープ
	ID3D12DescriptorHeap* ppHeap[] = { SRVHandler::GetInstance()->GetHeap() };
	commandList_->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	// テーブル設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(EffectRegister::kTexture), this->renderTextureHandle_.second);
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		commandList_, static_cast<UINT>(EffectRegister::kDissolveTexture), sDissolveTexture);

	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kVignette), vignetteCBuffer_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kBlur), blurCBuffer_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kDissolve), dissolveCBuffer_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kNoise), noiseCBuffer_->GetGPUVirtualAddress());

	// 描画
	commandList_->DrawInstanced(3, 1, 0, 0);

}
