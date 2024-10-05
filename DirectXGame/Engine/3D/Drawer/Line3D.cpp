#include "Line3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

void Line3D::Intialize()
{
	vertexData_.resize(size_t(1024));

	CreateVertex();
}

void Line3D::Update()
{

	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(LineData) * vertexData_.size());
}

void Line3D::Draw(ICamera* camera)
{
	// 頂点情報とかも送らないと動かない
	ModelRenderer::LineDraw(camera, this);
}

void Line3D::CreateVertex()
{
	// 配列のサイズがない場合勝手にリサイズ
	if (vertexData_.size() == 0) {
		vertexData_.resize(size_t(2));
	}
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点作成
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(LineData) * vertexData_.size());
	// バッファービュー作成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(LineData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(LineData);
}

void Line3D::RefreshVertex()
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// マッピングのし直し
	vertex_.cBuffer->Unmap(0, nullptr);
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(LineData) * vertexData_.size());
	// ビューの再生成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(LineData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(LineData);
}
