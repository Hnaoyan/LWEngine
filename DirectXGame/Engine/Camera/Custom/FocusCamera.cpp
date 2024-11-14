#include "FocusCamera.h"

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
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
		UpdateView(cameraMatrix);
	}
	else {
		ICamera::Update();
	}

}

void FocusCamera::ImGuiDraw()
{
}

void FocusCamera::FocusUpdate()
{
	// 対象地点があれば
	if (worldTransform_) {
		transform_.translate.y = worldTransform_->GetWorldPosition().y;
		Vector3 focusDirect = (*focusPoint_) - transform_.translate;
		focusDirect = focusDirect.Normalize();
		rotateQuaternion_ = Quaternion::MakeRotateToDirect(focusDirect);
	}
}
