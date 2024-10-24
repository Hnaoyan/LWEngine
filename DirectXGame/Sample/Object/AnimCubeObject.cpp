#include "AnimCubeObject.h"
#include "imgui.h"

int32_t AnimCubeObject::sSerialNumber = 0u;

void AnimCubeObject::Initialize(Model* model)
{
	animModel_ = model;
	animData_ = animModel_->GetModelData()->animData;
	// 番号
	serialNumber_ = sSerialNumber;
	sSerialNumber++;

	// 名前
	objectName_ = "AnimCube" + std::to_string(serialNumber_);

	worldTransform_.Initialize();
	// アニメーションプレイヤー
	animPlayer_.Initialize(&animData_);
}

void AnimCubeObject::Update()
{
	animPlayer_.Update();
	//animationTime_ -= 1.0f / 60.0f;
	////animationTime_ = std::fmod(animationTime_, animData_.duration);
	//	 
	//if (animationTime_ < 0) {
	//	animationTime_ = animData_.duration;
	//}
	//
	animationTime_ = animPlayer_.animationTime_;
	// ノード取得
	NodeAnimation& nodeAnim = animData_.nodeAnimations[animModel_->GetModelData()->rootNode.name];

	// トランスフォームの取得
	transform_.translate = Animation::CalculateValue(nodeAnim.translate.keyframes, animationTime_);
	transform_.rotate = Animation::CalculateValue(nodeAnim.rotate.keyframes, animationTime_);
	transform_.scale = Animation::CalculateValue(nodeAnim.scale.keyframes, animationTime_);
	
	Quaternion normalizeRotate = Quaternion::Normalize(transform_.rotate);

	// ローカル行列
	localMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, normalizeRotate, transform_.translate);

	worldTransform_.localMatrix_ = localMatrix_;
	worldTransform_.UpdateMatrix();
}

void AnimCubeObject::Draw(const ModelDrawDesc& desc)
{
	ModelDrawDesc drawDesc = ModelDrawDesc::SetDesc(desc);
	drawDesc.worldTransform = &worldTransform_;

	animModel_->Draw(drawDesc);
}

void AnimCubeObject::ImGuiDraw()
{
	ImGui::Begin(objectName_.c_str());
	std::string name = "AnimTimer:" + std::to_string(serialNumber_);
	ImGui::DragFloat(name.c_str(), &animationTime_);
	name = "Position:" + std::to_string(serialNumber_);
	ImGui::DragFloat3(name.c_str(), &worldTransform_.transform_.translate.x, 0.01f);
	animPlayer_.ImguiDraw(objectName_.c_str());
	ImGui::End();
}
