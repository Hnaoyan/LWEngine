#include "SampleScene.h"
#include "imgui.h"
#include "../../Engine/2D/TextureManager.h"

void SampleScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// モデル読み込み
	LoadModel();
	// テクスチャ関係読み込み
	LoadTexture();

	//---ここから書く---//


}

void SampleScene::Update()
{
	newSprite_->SetPosition(position_);

	
	// カメラの更新
	CameraUpdate();
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

	Model::PreDraw(commandList);

	testModel_->Draw(testWTF_, &camera_);

	Model::PostDraw();

#pragma region スプライト

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion

}

void SampleScene::ImGuiDraw()
{
	ImGui::Begin("Test");
	ImGui::DragFloat2("pos", &position_.x, 0.01f);
	ImGui::End();
	ImGui::ShowDemoWindow();
}

void SampleScene::LoadModel()
{
	testModel_.reset(Model::CreateObj("cube"));
}

void SampleScene::LoadTexture()
{
	uvTexture_ = TextureManager::Load("Resources/default/uvChecker.png");
	newSprite_.reset(Sprite::Create(uvTexture_, { 300.0f,0.0f }, { 0,0 }));
}

void SampleScene::CameraUpdate()
{
	// 基底更新
	IScene::CameraUpdate();
}
