#include "InstancedGroup.h"
#include "../Descriptor/SRVHandler.h"
#include "../../2D/TextureManager.h"

#include <cassert>
#include <imgui.h>

void InstancedGroup::Initialize(Model* model)
{
	// モデルの設定
	assert(model);
	model_ = model;

	// データの作成
	CreateData();
}

void InstancedGroup::Update()
{
	unitNum_ = 0;
	for (std::vector<std::unique_ptr<InstancedUnit>>::iterator it = units_.begin();
		it != units_.end(); ++it) {

		(*it)->Update();
		// 行列の処理
		unitDataMap_[unitNum_].worldMatrix = (*it)->GetWorldMatrix();
		unitDataMap_[unitNum_].worldInverseTranspose = Matrix4x4::MakeTranspose(Matrix4x4::MakeInverse((*it)->GetWorldMatrix()));
		unitDataMap_[unitNum_].instancedNum = unitNum_;
		// イテレート
		unitNum_++;
	}

}

void InstancedGroup::Draw(ModelDrawDesc desc)
{
	// 描画
	model_->InstancedDraw(desc, unitNum_, srvHandles_.second);
}

void InstancedGroup::UnitRegist(const Vector3& position)
{
	std::unique_ptr<InstancedUnit> instance = std::make_unique<InstancedUnit>();
	instance->Intialize();
	instance->transform_.translate = position;
	instance->Update();
	units_.push_back(std::move(instance));
}

void InstancedGroup::ImGuiDraw()
{
	std::string name = "Group" + std::to_string(srvIndex);
	ImGui::Begin(name.c_str());
	name = "Position" + std::to_string(srvIndex);
	ImGui::DragFloat3(name.c_str(), &registPosition_.x, 0.01f);
	name = "Add" + std::to_string(srvIndex);
	if (ImGui::Button(name.c_str())) {
		this->UnitRegist(registPosition_);
	}

	ImGui::End();
}

void InstancedGroup::CreateData()
{
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	// インスタンシング用
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstanceMax;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(CBufferInstancedUnit);

	// リソース作成
	groupResources = DxCreateLib::ResourceLib::CreateBufferResource(device, (sizeof(CBufferInstancedUnit) * kNumInstanceMax));
	// 書き込み
	groupResources->Map(0, nullptr, reinterpret_cast<void**>(&unitDataMap_));
	for (uint32_t i = 0; i < kNumInstanceMax; ++i) {
		unitDataMap_[i].worldMatrix = Matrix4x4::MakeIdentity4x4();
		unitDataMap_[i].worldInverseTranspose = Matrix4x4::MakeIdentity4x4();
		unitDataMap_[i].instancedNum = i;
	}
	// SRVの設定
	srvHandles_.first = SRVHandler::GetSrvHandleCPU();
	srvHandles_.second = SRVHandler::GetSrvHandleGPU();
	srvIndex = SRVHandler::AllocateDescriptor();
	// SRV作成
	device->CreateShaderResourceView(groupResources.Get(), &instancingSrvDesc, srvHandles_.first);

}
