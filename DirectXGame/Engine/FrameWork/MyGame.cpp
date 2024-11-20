#include "MyGame.h"
#include "../Scene/SceneFactory/SceneFactory.h"
#include "../GlobalVariables/GlobalVariables.h"
#include "../PostEffect/PostEffectRender.h"

#include "imgui.h"

void MyGame::Initialize()
{
	// 基底クラス
	Framework::Initialize();
	// ロード
	GlobalVariables::GetInstance()->LoadFiles();
	// 初期化
	PostEffectRender::GetInstance()->StaticInitialize();

	// シーンの初期化
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	sceneManager_->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	sceneFactory_.release();
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
	// RenderTextureの描画前
	dxCommon_->RenderPreDraw();
	// シーンの描画
	sceneManager_->Draw();
	// RenderTextureの描画後
	dxCommon_->RenderPostDraw();

	// 描画前処理
	dxCommon_->PreDraw();
	// PostEffectの描画
	PostEffectRender::GetInstance()->Draw(dxCommon_->GetCommandList());
	// PostEffectを掛けないUIの描画
	sceneManager_->UIDraw();
	// ImGuiの描画
	ImGuiDraw();
	// 描画後処理
	dxCommon_->PostDraw();
}

void MyGame::ImGuiDraw()
{
	// 受付開始
	imGuiManager_->Begin();
	// グローバル変数群
	GlobalVariables::GetInstance()->Update();
	// シーンのImGuiまとめた関数呼び出し
	sceneManager_->ImGuiDraw();
	// 受付終了
	imGuiManager_->End();
	// 描画
	imGuiManager_->Draw();
}
