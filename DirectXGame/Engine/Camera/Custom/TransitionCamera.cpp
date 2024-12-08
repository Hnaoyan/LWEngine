#include "TransitionCamera.h"

void TransitionCamera::Initialize()
{
	ICamera::Initialize();
}

void TransitionCamera::Update()
{
	transitionTimer_.Update();

	if (transitionTimer_.IsActive()) {
		// 座標の移動
		transform_.translate = Ease::Easing(easePoints_.first, easePoints_.second, transitionTimer_.GetElapsedFrame());

		// 回転の処理
		Vector3 focusDirect = Vector3(focusPoint_ - transform_.translate).Normalize();
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix(transform_.scale, Quaternion::MakeRotateDirect(focusDirect), transform_.translate);
		UpdateView(cameraMatrix);
	}

}

void TransitionCamera::ImGuiDraw()
{
}

void TransitionCamera::BeginTransition(const Vector3& startPoint, const Vector3& endPoint, const float& frame)
{
	easePoints_.first = startPoint;
	easePoints_.second = endPoint;
	transitionTimer_.Start(frame);
}
