#include "FollowCamera.h"
#include "../../Input/Input.h"
#include "imgui.h"
#include "../../LwLib/LwEngineLib.h"
#include "Application/GameObject/Player/System/LockOn.h"
#include "Application/GameObject/Enemy/SampleEnemy.h"

void FollowCamera::Initialize()
{
	// 初期化
	ICamera::Initialize();
	defaultOffset_ = { 0.0f,2.0f,-10.0f };

	rStickRotateSpeed_ = 0.1f;
	rStickLerpRate_ = 0.1f;
	delayRate_ = 0.1f;
}

void FollowCamera::Update()
{
	// コントローラー
	XINPUT_STATE joyState;

	// 追尾
	if (target_) {
		// 入力クラス
		Input* input = Input::GetInstance();
		// スティック操作
		if (input->GetJoystickState(0, joyState)) {
			// 目標回転角の設定
			destinationAngle_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rStickRotateSpeed_;
			//destinationAngle_.z += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rStickRotateSpeed_;
		}

		if (lockOn_->ExistTarget()) {
			// ロックオンしたオブジェクトの座標
			Vector3 lockOnPosition = lockOn_->GetTarget()->worldTransform_.GetWorldPosition();
			// 追尾してるオブジェクトの座標
			Vector3 sub = lockOnPosition - target_->GetWorldPosition();
			sub = Vector3::Normalize(sub);
			// Y軸角度
			destinationAngle_.y = LwLib::CalculateYawFromVector({ sub.x,0,sub.z });
			//destinationAngle_.y = std::atan2f(sub.x, sub.z);
		}

		// 回転の速度調節
		transform_.rotate.y = LwLib::Lerp(transform_.rotate.y, destinationAngle_.y, rStickLerpRate_);
		transform_.rotate.z = LwLib::LerpShortAngle(transform_.rotate.z, destinationAngle_.z, rStickLerpRate_);

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

	ImGui::DragFloat("rStick", &rStickRotateSpeed_, 0.01f);

	ImGui::DragFloat("rStickRate", &rStickLerpRate_, 0.01f);

	ImGui::DragFloat("DeleyRate", &delayRate_, 0.01f);

	ImGui::DragFloat3("Front", &frontVector_.x);

	ImGui::End();
}

void FollowCamera::Reset()
{
	if (target_) {
		// 追従対象・角度初期化
		Vector3 worldPosition = target_->GetWorldPosition();
		interTarget_ = worldPosition;
		transform_.rotate.y = target_->transform_.rotate.y;
		transform_.rotate.z = target_->transform_.rotate.z;
	}
	// 目標角の設定
	destinationAngle_.y = transform_.rotate.y;
	destinationAngle_.z = transform_.rotate.z;
	// 追従対象からのオフセット
	Vector3 offset = CreateOffset();
	transform_.translate = interTarget_ + offset;
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
