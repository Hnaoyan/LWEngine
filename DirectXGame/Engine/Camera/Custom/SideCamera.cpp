#include "SideCamera.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <imgui.h>

void SideCamera::Initialize()
{
	// 基底
	ICamera::Initialize();

	// トランスフォーム
	worldTransform_.Initialize();
	worldTransform_.transform_.translate = { 15.0f,1.0f,0.0f };
	worldTransform_.UpdateMatrix();
}

void SideCamera::Update()
{
	// チェック対象がある場合
	if (worldTransform_.parent_) {
		// 対象に向ける
		Vector3 direct = worldTransform_.parent_->GetWorldPosition() - worldTransform_.GetWorldPosition();
		// Y軸回転で向ける
		worldTransform_.transform_.rotate.y = LwLib::CalculateYawFromVector(Vector3(direct.Normalize().x, 0.0f, direct.Normalize().z));
	}
	// 更新
	worldTransform_.UpdateMatrix();

	// カメラのトランスフォームに設定
	transform_.rotate = worldTransform_.transform_.rotate;
	transform_.translate = worldTransform_.GetWorldPosition();
	// 基底
	ICamera::Update();
}

void SideCamera::ImGuiDraw()
{
	ImGui::DragFloat3("Position", &worldTransform_.transform_.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
}

void SideCamera::InitializeGlobalValue()
{
}
