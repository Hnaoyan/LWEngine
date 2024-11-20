#include "SkyDomeObject.h"
#include "imgui.h"

#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void SkyDomeObject::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = { 500.0f,500.0f,500.0f };
	worldTransform_.UpdateMatrix();
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();

	// 外部書き出しの初期化
	InitializeGlobalValue();
}

void SkyDomeObject::Update()
{
	// マテリアル
	material_->Update();
	// 行列更新
	worldTransform_.UpdateMatrix();
}

void SkyDomeObject::ImGuiDraw()
{
	ImGui::Checkbox("IsInvisible", &isInvisible_);
	float scale = worldTransform_.transform_.scale.x;
	ImGui::DragFloat("Scale", &scale, 0.1f);
	worldTransform_.transform_.scale = { scale,scale,scale };
	worldTransform_.UpdateMatrix();
	ImGui::ColorEdit4("Color", &material_->color_.x);
}

void SkyDomeObject::Draw(ModelDrawDesc desc)
{
	if (isInvisible_) {
		return;
	}
	// マテリアル更新
	model_->GetMaterial()->Update();
	// デスクの設定
	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();
	// プレイヤーの描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);

}

void SkyDomeObject::InitializeGlobalValue()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "Skydome";
	// 生成
	global->CreateGroup(groupName);
	global->AddValue(groupName, "Translate", Vector3(1.0f, 1.0f, 1.0f));
	global->AddValue(groupName, "Scale", Vector3(1.0f,1.0f,1.0f));
	global->AddValue(groupName, "Color", Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	// 更新
	GlobalValueUpdate();
}

void SkyDomeObject::GlobalValueUpdate()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string groupName = "Skydome";
	
	worldTransform_.transform_.translate = global->GetValue<Vector3>(groupName, "Translate");
	worldTransform_.transform_.scale = global->GetValue<Vector3>(groupName, "Scale");
	Vector4 color = global->GetValue<Vector4>(groupName, "Color");
	material_->color_ = color;
}
