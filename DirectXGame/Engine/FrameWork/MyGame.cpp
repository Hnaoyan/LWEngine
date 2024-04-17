#include "MyGame.h"

void MyGame::Initialize()
{
	// 基底クラス
	Framework::Initialize();



}

void MyGame::Finalize()
{
	// 基底フラグ
	Framework::Finalize();
}

void MyGame::Update()
{
	// 基底クラス
	Framework::Update();

	// 終了フラグ
	endRequest_ = winApp_->ProcessMessage();

}

void MyGame::Draw()
{
	// 描画前処理
	dxCommon_->PreDraw();


	//ImGuiDraw();

	// 描画後処理
	dxCommon_->PostDraw();
}

void MyGame::ImGuiDraw()
{

}
