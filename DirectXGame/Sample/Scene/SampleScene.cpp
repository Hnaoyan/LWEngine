#include "SampleScene.h"
#include "imgui.h"
#include "../../Engine/2D/TextureManager.h"

void SampleScene::Initialize()
{
	IScene::Initialize();

	uvTexture_ = TextureManager::Load("Resources/default/uvChecker.png");
	newSprite_.reset(Sprite::Create(uvTexture_, { 300.0f,0.0f }, { 0,0 }));
}

void SampleScene::Update()
{
	newSprite_->SetPosition(position_);
}

void SampleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region スプライト
		
	Sprite::PreDraw(commandList);

	newSprite_->Draw();

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
	ImGui::DragFloat2("pos", &position_.x, 0.01f);
	ImGui::End();
	ImGui::ShowDemoWindow();
}
