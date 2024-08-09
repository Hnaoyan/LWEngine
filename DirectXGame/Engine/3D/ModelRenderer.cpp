#include "ModelRenderer.h"
#include <cassert>

ID3D12GraphicsCommandList* ModelRenderer::sCommandList_ = nullptr;

void ModelRenderer::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	assert(cmdList);
	// コマンドリストの設定
	sCommandList_ = cmdList;
}

void ModelRenderer::PostDraw()
{
	// コマンドリストの解除
	sCommandList_ = nullptr;
}
