#include "FollowCamera2D.h"
#include "Engine/LwLib/LwLibrary.h"

void FollowCamera2D::Initialize()
{
	// 初期化
	ICamera::Initialize();

	delayRate_ = 0.1f;
}

void FollowCamera2D::Update()
{
	if (target_) {
		FollowUpdate();
	}

	ICamera::Update();
}

void FollowCamera2D::ImGuiDraw()
{

}

void FollowCamera2D::FollowUpdate()
{

	interTarget_ = Vector3::Lerp(interTarget_, target_->GetWorldPosition(), delayRate_);
	interTarget_.z = offsetZ;

	transform_.translate = interTarget_;

}
