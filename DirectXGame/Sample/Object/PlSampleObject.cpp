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

void PlSampleObject::Draw(ICamera* camera)
{
	ModelDrawDesc desc{};
	desc.worldTransform = &worldTransform_;
	desc.camera = camera;

	model_->Draw(desc);

}

void PlSampleObject::ImGuiDraw()
{
	ImGui::Begin("Player");

	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);

	ImGui::End();
}
