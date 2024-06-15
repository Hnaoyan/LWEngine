#include "PostEffectRender.h"
#include "../3D/Descriptor/SRVHandler.h"
#include "../3D/Graphics/GraphicsPSO.h"

void PostEffectRender::Draw(ID3D12GraphicsCommandList* cmdList)
{
	commandList_ = cmdList;

	// PSO
	commandList_->SetPipelineState(GraphicsPSO::sPostEffectPipelineStates_[size_t(Pipeline::PostEffectType::kNormal)].Get());
	commandList_->SetGraphicsRootSignature(GraphicsPSO::sPostEffectRootSignature_.Get());

	// SRVヒープ
	ID3D12DescriptorHeap* ppHeap[] = { SRVHandler::GetInstance()->GetSrvHeap() };
	commandList_->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	// テーブル設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->SetGraphicsRootDescriptorTable(0, this->renderTextureHandle_.second);

	// 描画
	commandList_->DrawInstanced(3, 1, 0, 0);

}
