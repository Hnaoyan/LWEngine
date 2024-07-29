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
	Vector2 rightStick = Input::GetInstance()->XGetRightJoystick();

	// 追尾
	if (target_) {
		// 入力クラス
		// 目標回転角の設定
		destinationAngle_.y += rightStick.x * rStickRotateSpeed_;
		destinationAngle_.x -= rightStick.y * rStickRotateSpeed_;

		if (lockOn_->ExistTarget()) {
			// ロックオンしたオブジェクトの座標
			Vector3 lockOnPosition = lockOn_->GetTarget()->worldTransform_.GetWorldPosition();
			// 追尾してるオブジェクトの座標
			Vector3 sub = lockOnPosition - target_->GetWorldPosition();
			sub = Vector3::Normalize(sub);
			if (isAtan_) {
				destinationAngle_.y = std::atan2f(sub.x, sub.z);
			}
			else {
				destinationAngle_.y = LwLib::CalculateYawFromVector({ sub.x,0,sub.z });
			}
			// Y軸角度
			//transform_.rotate.x = std::atan2f(std::sqrtf(std::powf(sub.x, 2) + std::powf(sub.z, 2)), sub.y);
			destinationAngle_.x = -std::atan2f(sub.y, std::sqrtf(std::powf(sub.x, 2) + std::powf(sub.z, 2)));
			transform_.rotate.y = destinationAngle_.y;
			transform_.rotate.x = destinationAngle_.x;
		}
		else {
			transform_.rotate.y = LwLib::Lerp(transform_.rotate.y, destinationAngle_.y, rStickLerpRate_);
			transform_.rotate.x = LwLib::Lerp(transform_.rotate.x, destinationAngle_.x, rStickLerpRate_);
		}

		// 回転の速度調節
		float thValue = 0.4f;
		transform_.rotate.x = std::clamp(transform_.rotate.x, -thValue, thValue);

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

	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);

	ImGui::DragFloat("rStick", &rStickRotateSpeed_, 0.01f);

	ImGui::DragFloat("rStickRate", &rStickLerpRate_, 0.01f);

	ImGui::DragFloat("DeleyRate", &delayRate_, 0.01f);

	ImGui::DragFloat3("Front", &frontVector_.x);

	ImGui::Checkbox("AtanCheck", &isAtan_);

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
