#include "InstancedGroup.h"
#include "../Descriptor/SRVHandler.h"
#include "Engine/2D/TextureManager.h"
#include "Engine/3D/ModelRenderer.h"

#include <cassert>
#include <imgui.h>

InstancedGroup::~InstancedGroup()
{
	// パーティクル関係
	SRVHandler::ReleaseHeapIndex(buffer_.data_.index);
}

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
		buffer_.cMap_[unitNum_].worldMatrix = (*it)->GetWorldMatrix();
		buffer_.cMap_[unitNum_].worldInverseTranspose = Matrix4x4::MakeTranspose(Matrix4x4::MakeInverse((*it)->GetWorldMatrix()));
		buffer_.cMap_[unitNum_].instancedNum = unitNum_;
		// イテレート
		unitNum_++;
	}

}

void InstancedGroup::Draw(ModelDrawDesc desc)
{
	// マテリアル更新
	model_->GetMaterial()->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	// 描画
	ModelRenderer::InstancedDraw(desc.camera, modelDesc, lightDesc, unitNum_, buffer_.GetSRVGPU());
}

void InstancedGroup::UnitRegist(const Vector3& position)
{
	std::unique_ptr<InstancedUnit> instance = std::make_unique<InstancedUnit>();
	instance->Initialize();
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

void InstancedGroup::DeleteUnit()
{
	// 死亡処理
	units_.erase(std::remove_if(units_.begin(), units_.end(), [](const std::unique_ptr<InstancedUnit>& obj) {
		return obj->IsDead();
		}), units_.end());
}

void InstancedGroup::CreateData()
{
	buffer_.CreateBuffer(DirectXCommon::GetInstance()->GetDevice(), kNumInstanceMax);

	for (uint32_t i = 0; i < kNumInstanceMax; ++i) {
		buffer_.cMap_[i].worldMatrix = Matrix4x4::MakeIdentity4x4();
		buffer_.cMap_[i].worldInverseTranspose = Matrix4x4::MakeIdentity4x4();
		buffer_.cMap_[i].instancedNum = i;
	}
}
