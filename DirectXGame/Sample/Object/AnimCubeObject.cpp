#include "AnimCubeObject.h"
#include "imgui.h"

void AnimCubeObject::Initialize(Model* model)
{
	animModel_ = model;
	animData_ = animModel_->GetModelData()->animData;
	//// 番号
	//serialNumber_ = sSerialNumber;
	//sSerialNumber++;

	//// 名前
	//objectName_ = sObjectName + std::to_string(serialNumber_);

	worldTransform_.Initialize();


}

void AnimCubeObject::Update()
{
	animationTime_ += 1.0f / 60.0f;
	animationTime_ = std::fmod(animationTime_, animData_.duration);
	// ノード取得
	NodeAnimation& nodeAnim = animData_.nodeAnimations[animModel_->GetModelData()->rootNode.name];

	// トランスフォームの取得
	transform_.translate = Animation::CalculateValue(nodeAnim.translate.keyframes, animationTime_);
	transform_.rotate = Animation::CalculateValue(nodeAnim.rotate.keyframes, animationTime_);
	transform_.scale = Animation::CalculateValue(nodeAnim.scale.keyframes, animationTime_);
	
	Quaternion normalizeRotate = Quaternion::Normalize(transform_.rotate);

	// ローカル行列
	localMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, normalizeRotate, transform_.translate);

	worldTransform_.worldMatrix_ = Matrix4x4::Multiply(localMatrix_, worldTransform_.worldMatrix_);
	worldTransform_.UpdateMatrix();
}

void AnimCubeObject::Draw(ICamera* camera)
{
	ModelDrawDesc desc;
	desc.worldTransform = &worldTransform_;
	desc.camera = camera;
	animModel_->Draw(desc);
}

void AnimCubeObject::ImGuiDraw()
{
	ImGui::Begin("cubeAnim");

	ImGui::DragFloat("AnimTimer", &animationTime_);
	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);

	ImGui::End();
}
