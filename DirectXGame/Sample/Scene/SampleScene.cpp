#include "SampleScene.h"
#include "imgui.h"
#include "../../Engine/2D/TextureManager.h"

void SampleScene::Initialize()
{
	IScene::Initialize();

	uvTexture_ = TextureManager::Load("Resources/default/uvChecker.png");
	uvSprite_.reset(Sprite::Create(uvTexture_, { 100,100 }));

}

void SampleScene::Update()
{

}

void SampleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region スプライト

	Sprite::PreDraw(commandList);

	uvSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

#pragma region スプライト

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion

}

void SampleScene::ImGuiDraw()
{
	ImGui::Begin("TEst");

	ImGui::End();
	ImGui::ShowDemoWindow();
}
