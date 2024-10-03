#include "Line3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

void Line3D::Intialize()
{
	vertexData_.resize(size_t(4));

	vertexData_[0].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[0].color = { 0.0f,1.0f,0.0f,1.0f };

	vertexData_[1].position = { 2.0f,0.0f,0.0f,1.0f };
	vertexData_[1].color = { 1.0f,0.0f,0.0f,1.0f };

	vertexData_[2].position = { 0.0f,2.0f,0.0f,1.0f };
	vertexData_[2].color = { 0.0f,0.0f,1.0f,1.0f };

	vertexData_[3].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[3].color = { 1.0f,1.0f,0.0f,1.0f };

	CreateVertex();
}

void Line3D::Update()
{

	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(Line3D) * vertexData_.size());
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
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TrailVertex) * vertexData_.size());
	// バッファービュー作成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TrailVertex) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TrailVertex);
}

void Line3D::RefreshVertex()
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// マッピングのし直し
	vertex_.cBuffer->Unmap(0, nullptr);
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TrailVertex) * vertexData_.size());
	// ビューの再生成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TrailVertex) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TrailVertex);
}
