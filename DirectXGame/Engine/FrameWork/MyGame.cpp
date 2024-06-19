#include "MyGame.h"
#include "../Scene/SceneFactory/SceneFactory.h"
#include "imgui.h"

void MyGame::Initialize()
{
	// 基底クラス
	Framework::Initialize();

	// シーンの初期化
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	sceneManager_->ChangeScene("SAMPLE");

}

void MyGame::Finalize()
{
	sceneFactory_.release();
	// 基底フラグ
	Framework::Finalize();
}

void MyGame::Update()
{
	// 基底クラス
	Framework::Update();

	// 入力受付
	input_->Update();

	// シーンの更新
	sceneManager_->Update();

	// 終了フラグ
	endRequest_ = winApp_->ProcessMessage();

}

void MyGame::Draw()
{
	// RenderGraph

	// 描画前処理
	dxCommon_->PreDraw();

	// シーンの描画
	sceneManager_->Draw();

	// ImGuiの描画
	ImGuiDraw();
	// 描画後処理
	dxCommon_->PostDraw();
}

void MyGame::ImGuiDraw()
{
	// 受付開始
	imGuiManager_->Begin();
	// シーンのImGuiまとめた関数呼び出し
	sceneManager_->ImGuiDraw();
	// 受付終了
	imGuiManager_->End();
	// 描画
	imGuiManager_->Draw();
}
