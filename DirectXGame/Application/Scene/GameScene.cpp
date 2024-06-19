#include "GameScene.h"

void GameScene::Initialize()
{
	// 基底クラス初期化
	IScene::Initialize();
	// モデル読み込み
	LoadModel();
	// テクスチャ関係読み込み
	LoadTexture();

}

void GameScene::Update()
{
	// カメラの更新
	CameraUpdate();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景

	Sprite::PreDraw(commandList);



	Sprite::PostDraw();

#pragma endregion
	// 深度クリア
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	Model::PostDraw();

#pragma region UI

	Sprite::PreDraw(commandList);


	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ImGuiDraw()
{
}

void GameScene::LoadModel()
{
}

void GameScene::LoadTexture()
{
}

void GameScene::CameraUpdate()
{
	// 基底更新
	IScene::CameraUpdate();
}
