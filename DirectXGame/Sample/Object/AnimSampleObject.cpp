#include "AnimSampleObject.h"
#include "imgui.h"

void AnimSampleObject::Initialize(Model* model)
{
	// モデル
	animModel_ = model;

	worldTransform_.Initialize();
	animation_.Initialize(animModel_->GetModelData());
	//animation_.modelData_ = &animModel_->GetModelData();
	//animation_.animData_ = animModel_->GetModelData().animData;
	animation_.animationTime_ = 60;
}

void AnimSampleObject::Update()
{

	// アニメーション更新
	animation_.UpdateAnimation();

	// アニメーションのローカル行列を
	//worldTransform_.localMatrix_ = animation_.localMatrix_;
	worldTransform_.UpdateMatrix();

}

void AnimSampleObject::Draw(ICamera* camera)
{
	ModelDrawDesc desc{};
	desc.worldTransform = &worldTransform_;
	desc.camera = camera;
	desc.localMatrix = animation_.localMatrix_;

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
