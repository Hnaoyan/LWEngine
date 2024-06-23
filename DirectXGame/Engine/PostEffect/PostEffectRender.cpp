#include "PostEffectRender.h"
#include "../3D/Descriptor/SRVHandler.h"

Pipeline::PostEffectType PostEffectRender::sPostEffect = Pipeline::PostEffectType::kNormal;

void PostEffectRender::Draw(ID3D12GraphicsCommandList* cmdList)
{
	commandList_ = cmdList;

	PostEffectPipeline pipeline = std::get<PostEffectPipeline>(GraphicsPSO::sPipelines_[size_t(Pipeline::Order::kPostEffect)]);
	// PSO
	commandList_->SetPipelineState(pipeline.pipelineStates[size_t(sPostEffect)].Get());
	commandList_->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	// SRVヒープ
	ID3D12DescriptorHeap* ppHeap[] = { SRVHandler::GetInstance()->GetSrvHeap() };
	commandList_->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	// テーブル設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->SetGraphicsRootDescriptorTable(0, this->renderTextureHandle_.second);

	// 描画
	commandList_->DrawInstanced(3, 1, 0, 0);

}
