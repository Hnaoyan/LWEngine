#include "SampleScene.h"
#include "imgui.h"


void SampleScene::Initialize()
{
	IScene::Initialize();


}

void SampleScene::Update()
{

}

void SampleScene::Draw()
{

}

void SampleScene::ImGuiDraw()
{
	ImGui::Begin("TEst");

	ImGui::End();
	ImGui::ShowDemoWindow();
}
