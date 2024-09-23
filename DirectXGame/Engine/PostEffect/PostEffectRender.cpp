#include "PostEffectRender.h"
#include "../3D/Descriptor/SRVHandler.h"
#include "../Base/DirectXCommon.h"
#include "Engine/2D/TextureManager.h"
#include <cassert>

Pipeline::PostEffectType PostEffectRender::sPostEffect = Pipeline::PostEffectType::kAlpha;
uint32_t PostEffectRender::sDissolveTexture = 0u;

void PostEffectRender::StaticInitialize()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// テクスチャ
	sDissolveTexture = TextureManager::Load("Resources/Dissolve/noise1.png");

	// リソース作成
	vignette_.CreateConstantBuffer(device);
	blur_.CreateConstantBuffer(device);
	dissolve_.CreateConstantBuffer(device);
	noise_.CreateConstantBuffer(device);
	hsv_.CreateConstantBuffer(device);
	bloom_.CreateConstantBuffer(device);

	vignette_.cMap_->scale = 16.0f;
	vignette_.cMap_->powValue = 0.8f;
	vignette_.cMap_->color = { 1.0f,0.0f,0.0f };

	blur_.cMap_->centerPoint = { 0.5f,0.5f };
	blur_.cMap_->samplesNum = 5;
	blur_.cMap_->blurWidth = 0.01f;

	dissolve_.cMap_->color = {};
	dissolve_.cMap_->threshold = 0;

	noise_.cMap_->time = 0;
	noise_.cMap_->enableScreen = 0;

	hsv_.cMap_->hue = 0.0f;
	hsv_.cMap_->saturation = 0.0f;
	hsv_.cMap_->value = 0.0f;

	bloom_.cMap_->threshold = 0.1f;
	bloom_.cMap_->sigma = 2.0f;
}

void PostEffectRender::Update(const PostEffectDesc& desc)
{
	// Vignette
	vignette_.cMap_->color = desc.vignette.color;
	vignette_.cMap_->powValue = desc.vignette.powValue;
	vignette_.cMap_->scale = desc.vignette.scale;

	// Blur
	blur_.cMap_->blurWidth = desc.blur.blurWidth;
	blur_.cMap_->centerPoint = desc.blur.centerPoint;
	blur_.cMap_->samplesNum = desc.blur.samplesNum;

	// Dissolve
	dissolve_.cMap_->color = desc.dissolve.color;
	dissolve_.cMap_->threshold = desc.dissolve.threshold;

	// HSV
	hsv_.cMap_->hue = desc.hsv.hue;
	hsv_.cMap_->saturation = desc.hsv.saturation;
	hsv_.cMap_->value = desc.hsv.value;

	// Bloom
	bloom_.cMap_->threshold = desc.bloom.threshold;
	bloom_.cMap_->sigma = desc.bloom.sigma;

	// ノイズでの処理時のみ
	if (sPostEffect != Pipeline::PostEffectType::kNoise) {
		noise_.cMap_->time = 0;
		return;
	}
	noise_.cMap_->time++;
	noise_.cMap_->enableScreen = desc.noise.enableScreen;
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
	// バッファーの設定
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kVignette), vignette_.cBuffer->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kBlur), blur_.cBuffer->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kDissolve), dissolve_.cBuffer->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kNoise), noise_.cBuffer->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kHSV), hsv_.cBuffer->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(EffectRegister::kBloom), bloom_.cBuffer->GetGPUVirtualAddress());

	// 描画
	commandList_->DrawInstanced(3, 1, 0, 0);
}
