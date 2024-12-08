#include "OrbitCamera.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <imgui.h>

void OrbitCamera::Initialize()
{
	ICamera::Initialize();

	transform_.translate = { 50.0f,50.0f,0.0f };
}

void OrbitCamera::Update()
{
	// 注視処理
	FocusUpdate();
}

void OrbitCamera::ImGuiDraw()
{
	static float dragRatio = 0.01f;
	ImGui::DragFloat("DragRatio", &dragRatio, 0.01f);
	ImGui::DragFloat3("Position", &transform_.translate.x, dragRatio);
	ImGui::DragFloat3("RotVector", &rotVector_.x, dragRatio);
	rotVector_ = rotVector_.Normalize();
	ImGui::DragFloat("RotRatio", &rotRatio_, dragRatio);
	rotRatio_ = std::clamp(rotRatio_, 0.0f, 1.0f);

	ImGui::DragFloat("FocusT", &focusT_, 0.01f);
	focusT_ = std::clamp(focusT_, 0.0f, 1.0f);
	ImGui::DragFloat("SlerpScale", &slerpScale_, 0.01f);
}

void OrbitCamera::FocusUpdate()
{
	if (objects_.first && objects_.second) {
		centerPoint_ = Ease::Easing(*objects_.first, *objects_.second, focusT_);
		//transform_.translate = LwLib::Slerp(Vector3::Normalize(*objects_.first), Vector3::Normalize(*objects_.second), rotRatio_) * slerpScale_;

	}
	// 対象地点があれば
	Vector3 focusDirect = (centerPoint_) - transform_.translate;	// 目的地点-自地点
	focusDirect = focusDirect.Normalize();
	rotateQuaternion_ = Quaternion::MakeRotateDirect(focusDirect);

	Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
	UpdateView(cameraMatrix);
}
