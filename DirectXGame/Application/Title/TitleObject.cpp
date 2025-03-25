#include "TitleObject.h"
#include "imgui.h"
#include "Engine/3D/ModelUtility/ModelRenderer.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/2D/TextureManager.h"

void TitleObject::Initialize(Model* model)
{
	IGameObject::Initialize(model);

	material_ = std::make_unique<Material>();
	material_->CreateMaterial();
	material_->color_.w = 0.75f;
	material_->discardThreshold_ = 0.1f;

}

void TitleObject::Update()
{
	if (isRotate_) {
		worldTransform_.transform_.rotate.y += (1.0f / 90.0f);
	}
	float turnAngle = 6.28f;
	if (worldTransform_.transform_.rotate.y >= turnAngle) {
		worldTransform_.transform_.rotate.y -= turnAngle;
	}
	IGameObject::Update();
}

void TitleObject::Draw(ModelDrawDesc desc)
{
	this->material_->Update();

	DrawDesc::LightDesc lightDesc{};
	DrawDesc::ModelDesc modelDesc{};
	lightDesc.directionalLight = desc.directionalLight;
	lightDesc.pointLight = desc.pointLight;
	lightDesc.spotLight = desc.spotLight;
	modelDesc.SetDesc(model_);
	modelDesc.worldTransform = &worldTransform_;
	modelDesc.material = material_.get();

	// 本体の描画
	ModelRenderer::NormalDraw(desc.camera, modelDesc, lightDesc);
}

void TitleObject::ImGuiDraw()
{
	ImGui::Begin("PlObject");
	static float dragValue = 0.01f;
	ImGui::DragFloat("DragV", &dragValue, 0.1f);
	ImGui::DragFloat3("Pos", &worldTransform_.transform_.translate.x, dragValue);
	ImGui::DragFloat3("Rot", &worldTransform_.transform_.rotate.x, dragValue);
	ImGui::Checkbox("IsRotate", &isRotate_);
	ImGui::End();
}
