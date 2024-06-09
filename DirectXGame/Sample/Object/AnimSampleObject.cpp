#include "AnimSampleObject.h"
#include "imgui.h"

int32_t AnimSampleObject::sSerialNumber = 0u;
std::string AnimSampleObject::sObjectName = "SampleObject";

void AnimSampleObject::Initialize(Model* model)
{
	// モデル
	animModel_ = model;

	// 番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 名前
	objectName_ = sObjectName + std::to_string(serialNumber_);

	worldTransform_.Initialize();
	animation_.Initialize(animModel_->GetModelDataAdress());
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

	animModel_->SkinningDraw(desc, &animation_);
}

void AnimSampleObject::ImGuiDraw()
{

	ImGui::Begin(objectName_.c_str());
	std::string name = "Position" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, 0.01f);
	name = "Rotate" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.rotate.x, 0.01f);
	name = "Scale" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.scale.x, 0.01f);
	ImGui::End();

}
