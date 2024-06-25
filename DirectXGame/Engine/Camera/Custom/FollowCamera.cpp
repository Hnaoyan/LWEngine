#include "FollowCamera.h"
#include "../../Input/Input.h"
#include "imgui.h"

void FollowCamera::Initialize()
{
	// 初期化
	ICamera::Initialize();
	defaultOffset_ = { 0.0f,2.0f,-10.0f };
}

void FollowCamera::Update()
{
	// コントローラー
	XINPUT_STATE joyState;

	// 追尾
	if (target_) {
		Input* input = Input::GetInstance();

		if (input->GetJoystickState(0, joyState)) {
			transform_.rotate.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 0.01f;
		}

		// 遅延追尾時の座標
		interTarget_ = Vector3::Lerp(interTarget_, target_->GetWorldPosition(), delayRate_);
		// オフセット作成
		Vector3 offset = CreateOffset();
		// カメラの座標作成
		transform_.translate = interTarget_ + offset;
	}

	ICamera::Update();
}

void FollowCamera::ImGuiDraw()
{
	ImGui::Begin("FollowCamera");

	ImGui::DragFloat3("Position", &transform_.translate.x);

	ImGui::DragFloat3("Rotate", &transform_.rotate.x);

	ImGui::DragFloat("DeleyRate", &delayRate_, 0.01f);

	ImGui::End();
}

void FollowCamera::Reset()
{
	if (target_) {
		// 追従対象・角度初期化
		Vector3 worldPosition = target_->GetWorldPosition();
		interTarget_ = worldPosition;
		//viewProjection_.rotation_.y = target_->rotation_.y;
	}

	//destinationAngleY_ = viewProjection_.rotation_.y;
	// 追従対象からのオフセット
	Vector3 offset = CreateOffset();
	transform_.translate = interTarget_ + offset;
	//transform_.translate += offset;

}

Vector3 FollowCamera::CreateOffset() const
{
	// カメラまでのオフセット
	Vector3 offset = defaultOffset_;

	// 回転行列の合成
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(
		Matrix4x4::MakeRotateXMatrix(transform_.rotate.x),
		Matrix4x4::Multiply(
			Matrix4x4::MakeRotateYMatrix(transform_.rotate.y),
			Matrix4x4::MakeRotateZMatrix(transform_.rotate.z)));
	offset = Matrix4x4::TransformVector3(offset, rotateMatrix);

	return Vector3(offset);
}
