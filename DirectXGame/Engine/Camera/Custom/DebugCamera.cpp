#include "DebugCamera.h"
#include "imgui.h"

void DebugCamera::Initialize()
{
	ICamera::Initialize();
	transform_.translate = { 0,0,-75.0f };

}

void DebugCamera::Update()
{


	UpdateMatrix();
	// 行列更新
	//ICamera::Update();
}

void DebugCamera::ImGuiDraw()
{
	//ImGui::Begin("DebugCamera");
	ImGui::DragFloat3("Position", &transform_.translate.x, 0.01f);
	ImGui::DragFloat4("Rotate", &transform_.rotate.x, 0.01f);
	//ImGui::End();
}
