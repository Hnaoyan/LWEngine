#include "MissileTrail.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"

#include <imgui.h>

void MissileTrail::Initialize()
{

	//vertexData_.resize(size_t(4));
	//vertexData_[0].position = { 0.0f,0.0f,0.0f,1.0f };
	//vertexData_[0].color = { 0.0f,1.0f,0.0f,1.0f };

	//vertexData_[1].position = { 2.0f,0.0f,0.0f,1.0f };
	//vertexData_[1].color = { 1.0f,0.0f,0.0f,1.0f };

	//vertexData_[2].position = { 0.0f,2.0f,0.0f,1.0f };
	//vertexData_[2].color = { 0.0f,0.0f,1.0f,1.0f };

	//vertexData_[3].position = { -2.0f,-2.0f,0.0f,1.0f };
	//vertexData_[3].color = { 1.0f,1.0f,0.0f,1.0f };

	CreateVertex();
}

void MissileTrail::Update()
{
	ImGui::Begin("Line");
	for (int i = 0; i < vertexData_.size(); ++i) {

		std::string name = "line" + std::to_string(i) + "Pos";

		ImGui::DragFloat4(name.c_str(), &vertexData_[i].position.x, 0.01f);
		name = "line" + std::to_string(i) + "Color";
		ImGui::ColorEdit4(name.c_str(), &vertexData_[i].color.x);
	}
	if (ImGui::Button("MultiPoint")) {
		//if(vertexData_[0].position == Vector4(0.0f,0.0f,0.0f,0.0f))

		TrailVertex point0{};
		point0.position = { 0.0f,1.0f,0.0f,1.0f };
		point0.color = { 1.0f,0.0f,0.0f,1.0f };
		vertexData_.push_back(point0);

		point0.position = { 0.0f,-1.0f,0.0f,1.0f };
		point0.color = { 0.0f,1.0f,0.0f,1.0f };
		vertexData_.push_back(point0);

		RefreshVertex();
	}
	if (ImGui::Button("SinglePoint")) {
		TrailVertex point0{};
		point0.position = { 0.0f,1.0f,0.0f,1.0f };
		point0.color = { 1.0f,0.0f,0.0f,1.0f };
		vertexData_.push_back(point0);

		RefreshVertex();
	}
	ImGui::End();

	// 書き込み？
	memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TrailVertex) * vertexData_.size());

	//for (int i = 0; i < vertexData_.size(); ++i) {
	//	vertex_.cMap_[i].position = vertexData_[i].position;
	//	vertex_.cMap_[i].color = vertexData_[i].color;
	//}

}

void MissileTrail::Draw(ICamera* camera)
{

	// 頂点情報とかも送らないと動かない
	ModelRenderer::TrailDraw(camera, this);
}

void MissileTrail::CreateVertex()
{
	if (vertexData_.size() == 0) {
		vertexData_.resize(size_t(2));
	}
	// 
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// 頂点作成
	{
		// 
		vertex_.CreateConstantBuffer(device, vertexData_.size());
		std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TrailVertex) * vertexData_.size());
		//vertex_.cBuffer->Unmap(0, nullptr);
		// 
		vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(TrailVertex) * vertexData_.size());
		vbView_.StrideInBytes = sizeof(TrailVertex);
	}
}

void MissileTrail::RefreshVertex()
{
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// マッピングのし直し
	vertex_.cBuffer->Unmap(0, nullptr);
	vertex_.CreateConstantBuffer(device, vertexData_.size());
	std::memcpy(vertex_.cMap_, vertexData_.data(), sizeof(TrailVertex) * vertexData_.size());
	
	// ビューの作成
	vbView_.BufferLocation = vertex_.cBuffer->GetGPUVirtualAddress();
	vbView_.SizeInBytes = UINT(sizeof(TrailVertex) * vertexData_.size());
	vbView_.StrideInBytes = sizeof(TrailVertex);
}
