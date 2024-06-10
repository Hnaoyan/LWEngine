#include "AnimSampleObject.h"
#include "imgui.h"

int32_t AnimSampleObject::sSerialNumber = 0u;
std::string AnimSampleObject::sObjectName = "SampleObject";

void AnimSampleObject::Initialize(Model* model, Model* cube)
{
	// モデル
	animModel_ = model;

	debugCube_ = cube;

	// 番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 名前
	objectName_ = sObjectName + std::to_string(serialNumber_);

	worldTransform_.Initialize();
	animation_.Initialize(animModel_->GetModelDataAdress());
	//animation_.modelData_ = &animModel_->GetModelData();
	//animation_.animData_ = animModel_->GetModelData().animData;
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


	//for (int i = 0; i < animation_.skeleton_.joints.size(); ++i) {
	//	Matrix4x4 local = animation_.skeleton_.joints[i].localMatrix;

	//	WorldTransform world = worldTransform_;
	//	world.worldMatrix_ = Matrix4x4::Multiply(world.worldMatrix_, local);

	//	desc.worldTransform = &world;
	//	desc.camera = camera;
	//	debugCube_->Draw(desc);
	//}

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

	animation_.ImGuiDraw();

	ImGui::End();

}
