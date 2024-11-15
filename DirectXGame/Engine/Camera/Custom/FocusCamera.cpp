#include "FocusCamera.h"
#include <imgui.h>

void FocusCamera::Initialize()
{
	ICamera::Initialize();
}

void FocusCamera::Update()
{

	// 視点の角度処理
	FocusUpdate();

	// バッファーに送る処理など
	if (isQuaternion_) {
		//Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
		//UpdateView(cameraMatrix);
	}
	else {
		ICamera::Update();
	}

}

void FocusCamera::ImGuiDraw()
{
	ImGui::DragFloat3("Position", &transform_.translate.x, 0.01f);	// 座標
}

void FocusCamera::FocusUpdate()
{
	// 対象地点があれば
	if (worldTransform_) {
		//transform_.translate.y = worldTransform_->GetWorldPosition().y;
		Vector3 focusDirect = (*focusPoint_) - transform_.translate;
		focusDirect = focusDirect.Normalize();
		rotateQuaternion_ = Quaternion::MakeRotateDirect(focusDirect);
		//Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(focusDirect);
		//Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate);
		//Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
		//Matrix4x4 cameraMatrix = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
		UpdateView(cameraMatrix);
	}
}
