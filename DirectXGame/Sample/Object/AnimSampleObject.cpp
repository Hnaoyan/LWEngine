#include "AnimSampleObject.h"
#include "imgui.h"

void AnimSampleObject::Initialize(Model* model)
{
	// モデル
	animModel_ = model;

	worldTransform_.Initialize();

}

void AnimSampleObject::Update()
{

	worldTransform_.UpdateMatrix();

}

void AnimSampleObject::Draw(ICamera* camera)
{
	ModelDrawDesc desc{};
	desc.worldTransform = &worldTransform_;
	desc.camera = camera;

	animModel_->Draw(desc);
}

void AnimSampleObject::ImGuiDraw()
{

	ImGui::Begin("Sample");
	ImGui::DragFloat3("pos", &worldTransform_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("rot", &worldTransform_.transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("sca", &worldTransform_.transform_.scale.x, 0.01f);
	ImGui::End();

}
