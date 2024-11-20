#include "FollowCamera.h"
#include "imgui.h"
#include "Engine/Input/Input.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/Player/System/PlayerSystemLists.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void FollowCamera::Initialize()
{
	// 初期化
	ICamera::Initialize();
	defaultOffset_ = { 0.0f,3.0f,-10.0f };
}

void FollowCamera::Update()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	rRotateSpeed_ = instance->GetValue<Vector2>("Camera", "RotateSpeed");
	rStickLerpRate_ = instance->GetValue<float>("Camera", "LerpRate");
	delayRate_ = instance->GetValue<float>("Camera", "DelayRate");
	xSpinLimit_ = instance->GetValue<float>("Camera", "XSpinLimit");
	defaultOffset_ = instance->GetValue<Vector3>("Camera", "Offset");

	// コントローラー
	Vector2 rightStick = GameSystem::sKeyConfigManager.GetKeyConfig()->rightStick;

	// 追尾
	if (target_) {
		// 入力クラス
		// 目標回転角の設定
		if (rightStick.x != 0.0f)
		{
			destinationAngle_.y += rightStick.x * rRotateSpeed_.x;
		}
		if (rightStick.y != 0.0f) {
			destinationAngle_.x -= rightStick.y * rRotateSpeed_.y;
		}

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
			destinationAngle_.x = -std::atan2f(sub.y, std::sqrtf(std::powf(sub.x, 2) + std::powf(sub.z, 2)));
			transform_.rotate.y = destinationAngle_.y;
			transform_.rotate.x = LwLib::Lerp(transform_.rotate.x, destinationAngle_.x, rStickLerpRate_);
		}
		else {
			transform_.rotate.y = LwLib::Lerp(transform_.rotate.y, destinationAngle_.y, rStickLerpRate_);
			transform_.rotate.x = LwLib::Lerp(transform_.rotate.x, destinationAngle_.x, rStickLerpRate_);
		}

		// 回転の速度調節
		destinationAngle_.x = std::clamp(destinationAngle_.x, -xSpinLimit_, xSpinLimit_);

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
	//ImGui::Begin("FollowCamera");
	ImGui::DragFloat("Fov", &fov_, 0.01f);
	ImGui::DragFloat3("Position", &transform_.translate.x);
	Vector3 world = GetWorldPosition();
	ImGui::DragFloat3("WorldPosition", &world.x);

	ImGui::DragFloat3("Offset", &defaultOffset_.x, 0.01f);
	float value = (transform_.rotate.x * (-10.0f) - 2.0f);
	//float rate = (0.3f - transform_.rotate.y);
	value = std::clamp(value, 0.0f, 1.0f);
	float z = LwLib::Lerp(-9.8f, -6.5f, value);
	ImGui::DragFloat("Rate", &value);
	ImGui::DragFloat("Z", &z);

	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);

	ImGui::DragFloat("rStick", &rStickRotateSpeed_, 0.01f);

	ImGui::DragFloat("rStickRate", &rStickLerpRate_, 0.01f);

	ImGui::DragFloat("xSpinLimit", &xSpinLimit_, 0.01f);

	ImGui::DragFloat("DeleyRate", &delayRate_, 0.01f);

	ImGui::DragFloat3("Front", &frontVector_.x);

	ImGui::Checkbox("AtanCheck", &isAtan_);

	if (ImGui::Button("Shake")) {
		ExecuteShake(15.0f, 5.0f);
	}

	//ImGui::End();
}

void FollowCamera::InitializeGlobalValue()
{
	GlobalVariables* instance = GlobalVariables::GetInstance();
	instance->CreateGroup("Camera");
	instance->AddValue("Camera", "RotateSpeed", rRotateSpeed_);
	instance->AddValue("Camera", "LerpRate", rStickLerpRate_);
	instance->AddValue("Camera", "DelayRate", delayRate_);
	instance->AddValue("Camera", "XSpinLimit", xSpinLimit_);
	instance->AddValue("Camera", "Offset", defaultOffset_);
	rRotateSpeed_ = instance->GetValue<Vector2>("Camera", "RotateSpeed");
	rStickLerpRate_ = instance->GetValue<float>("Camera", "LerpRate");
	delayRate_ = instance->GetValue<float>("Camera", "DelayRate");
	xSpinLimit_ = instance->GetValue<float>("Camera", "XSpinLimit");
	defaultOffset_ = instance->GetValue<Vector3>("Camera", "Offset");
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
	// y(-0.2~-0.3f)
	// offsetZ(-6.9~-9.8)
	float z = defaultOffset_.z;
	if (transform_.rotate.x <= -0.2f) {
		float value = (transform_.rotate.x * (-10.0f) - 2.0f);
		z = LwLib::Lerp(-9.8f, -6.5f, value);
	}

	// カメラまでのオフセット
	Vector3 offset = { defaultOffset_.x,defaultOffset_.y,z };

	// 回転行列の合成
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(
		Matrix4x4::MakeRotateXMatrix(transform_.rotate.x),
		Matrix4x4::Multiply(
			Matrix4x4::MakeRotateYMatrix(transform_.rotate.y),
			Matrix4x4::MakeRotateZMatrix(transform_.rotate.z)));
	offset = Matrix4x4::TransformVector3(offset, rotateMatrix);

	return Vector3(offset);
}
