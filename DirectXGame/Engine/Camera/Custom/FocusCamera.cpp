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
	ImGui::DragFloat("Fov", &fov_, 0.01f);
}

void FocusCamera::FocusUpdate()
{
	// 対象地点があれば
	if (worldTransform_) {
		//transform_.translate.y = worldTransform_->GetWorldPosition().y;
		Vector3 focusDirect = (*focusPoint_) - transform_.translate;
		focusDirect = focusDirect.Normalize();
		rotateQuaternion_ = Quaternion::MakeRotateDirect(focusDirect);

		//float dot = Vector3::Dot(Vector3::Forward(), focusDirect);
		//float theta = std::acosf(dot);
		//rotateQuaternion_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Cross(Vector3::Forward(), focusDirect).Normalize(), theta);
		
		//Vector3 xzNorm = focusDirect;
		//xzNorm.y = 0.0f;
		//xzNorm = xzNorm.Normalize();
		//float yAngle = std::acosf(Vector3::Dot(Vector3::Forward(), xzNorm));
		//Quaternion yRot = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Up(), yAngle);

		//float xAngle = std::acosf(Vector3::Dot(focusDirect, xzNorm));
		//Quaternion xRot = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::Right(), xAngle);
		//rotateQuaternion_ = yRot * xRot;

		//Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(focusDirect);
		//Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate);
		//Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
		//Matrix4x4 cameraMatrix = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
		UpdateView(cameraMatrix);
	}
}
