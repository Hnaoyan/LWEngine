#include "FocusCamera.h"

void FocusCamera::Initialize()
{
	ICamera::Initialize();
}

void FocusCamera::Update()
{
	// 対象地点があれば
	if (worldTransform_) {
		Vector3 focusDirect = (*focusPoint_) - transform_.translate;
		rotateQuaternion_ = Quaternion::MakeRotateToDirect(focusDirect);
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, rotateQuaternion_, transform_.translate);
		//// 回転
		//Matrix4x4 rotateMat = Matrix4x4::DirectionToDirection({ 0.0f,0.0f,1.0f }, Vector3::Normalize(focusDirect));
		//Matrix4x4 cameraMatrix = Matrix4x4::Multiply(Matrix4x4::MakeScaleMatrix(transform_.scale), Matrix4x4::Multiply(rotateMat, Matrix4x4::MakeTranslateMatrix(transform_.translate)));
		UpdateView(cameraMatrix);
	}
	else{
		ICamera::Update();
	}

}

void FocusCamera::ImGuiDraw()
{
}
