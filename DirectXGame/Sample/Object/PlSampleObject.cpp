#include "PlSampleObject.h"
#include "../../Engine/Input/Input.h"
#include "imgui.h"

void PlSampleObject::Initialize(Model* model)
{
	// モデル設定
	model_ = model;
	// 書
	worldTransform_.Initialize();
}

void PlSampleObject::Update()
{
	Input* input = Input::GetInstance();
	float speed = 0.1f;

	velocity_ = {};

	if (input->PressKey(DIK_A)) {
		velocity_.x -= speed;
	}
	else if (input->PressKey(DIK_D)) {
		velocity_.x += speed;
	}

	worldTransform_.transform_.translate += velocity_;
	worldTransform_.UpdateMatrix();
}

void PlSampleObject::Draw(const ModelDrawDesc& desc)
{
	ModelDrawDesc drawDesc{};
	drawDesc.worldTransform = &worldTransform_;
	drawDesc.camera = desc.camera;
	drawDesc.directionalLight = desc.directionalLight;
	drawDesc.spotLight = desc.spotLight;
	drawDesc.pointLight = desc.pointLight;

	model_->Draw(drawDesc);

}

void PlSampleObject::ImGuiDraw()
{
	ImGui::Begin("Player");

	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);

	ImGui::End();
}
