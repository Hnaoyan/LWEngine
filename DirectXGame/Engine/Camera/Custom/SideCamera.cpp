#include "SideCamera.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <imgui.h>

void SideCamera::Initialize()
{
	ICamera::Initialize();

	worldTransform_.Initialize();
	worldTransform_.transform_.translate = { 5.0f,1.0f,0.0f };
	worldTransform_.UpdateMatrix();
}

void SideCamera::Update()
{
	// 
	Vector3 direct = worldTransform_.parent_->GetWorldPosition() - worldTransform_.GetWorldPosition();

	worldTransform_.transform_.rotate.y = LwLib::CalculateYawFromVector(Vector3(direct.Normalize().x, 0.0f, direct.Normalize().z));
	worldTransform_.UpdateMatrix();

	transform_.rotate = worldTransform_.transform_.rotate;
	transform_.translate = worldTransform_.GetWorldPosition();
	// 基底
	ICamera::Update();
}

void SideCamera::ImGuiDraw()
{
	ImGui::DragFloat3("Rotate", &worldTransform_.transform_.rotate.x, 0.01f);
}

void SideCamera::InitializeGlobalValue()
{
}
