#include "SkyDomeObject.h"
#include "Engine/3D/ModelRenderer.h"
#include "imgui.h"

void SkyDomeObject::Initialize(Model* model)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.transform_.scale = { 500.0f,500.0f,500.0f };
	worldTransform_.UpdateMatrix();
	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
}

void SkyDomeObject::Update()
{
	material_->Update();
	// 行列更新
	worldTransform_.UpdateMatrix();
}

void SkyDomeObject::ImGuiDraw()
{
	ImGui::Begin("SkyDomeObject");
	float scale = worldTransform_.transform_.scale.x;
	ImGui::DragFloat("Scale", &scale, 0.1f);
	worldTransform_.transform_.scale = { scale,scale,scale };
	worldTransform_.UpdateMatrix();
	ImGui::DragFloat4("ModelColor", &material_->color_.x, 0.01f);
	ImGui::End();
}

void SkyDomeObject::Draw(ModelDrawDesc desc)
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
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();
	// プレイヤーの描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);

}
