#include "Triangle3D.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

void Triangle3D::Initialize()
{
	vertexData_.resize(size_t(3));

	vertexData_[0].position = { 0.0f,0.0f,0.0f };
	vertexData_[0].color = { 1.0f,0.0f,0.0f,1.0f };

	vertexData_[1].position = { 2.0f,-2.0f,0.0f };
	vertexData_[1].color = { 0.0f,1.0f,0.0f,1.0f };

	vertexData_[2].position = { -2.0f,-2.0f,0.0f };
	vertexData_[2].color = { 0.0f,0.0f,1.0f,1.0f };

	CreateVertex();
}

void Triangle3D::Update()
{
	// ポインタに値を渡す
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
}

void Triangle3D::Draw(ICamera* camera)
{
	ModelRenderer::TriangleDraw(camera, this);
}

void Triangle3D::CreateVertex()
{
	// 配列のサイズがない場合勝手にリサイズ
	if (vertexData_.size() == 0) {
		vertexData_.resize(size_t(2));
	}
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点作成
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	// バッファービュー作成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TriangleData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TriangleData);
}

void Triangle3D::RefreshVertex()
{
	// デバイス
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// マッピングのし直し
	vertex_.cBuffer->Unmap(0, nullptr);
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TriangleData) * vertexData_.size());
	// ビューの再生成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TriangleData) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TriangleData);
}
