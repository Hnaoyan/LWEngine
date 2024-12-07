#include "SceneTransitionManager.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/WindowAPI/WindowAPI.h"
#include <imgui.h>

SceneTransitionManager::SceneTransitionManager()
{
}

void SceneTransitionManager::Initialize()
{
	// 色
	sColor = { 0.0f,0.0f,0.0f,0.0f };

	// スプライト
	transitionSprite_ = SpriteManager::GetSprite("TransitionSprite");
	transitionSprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	Vector2 center = Vector2((float)WindowAPI::kWindowWidth, (float)WindowAPI::kWindowHeight);
	center /= 2.0f;
	transitionSprite_->SetPosition(center);
	transitionSprite_->SetSize(Vector2((float)WindowAPI::kWindowWidth, (float)WindowAPI::kWindowHeight));
}

void SceneTransitionManager::Update()
{
	// タイマーの更新
	setupTimer_.Update();
	returnTimer_.Update();

	// アルファ値の処理
	if (setupTimer_.IsActive()) {
		sColor.w = Ease::Easing(0.0f, 1.0f, setupTimer_.GetElapsedFrame());
	}
	else if(returnTimer_.IsActive()){
		sColor.w = Ease::Easing(1.0f, 0.0f, returnTimer_.GetElapsedFrame());
	}

	//// 切り返し処理
	//if (setupTimer_.IsEnd()) {
	//	returnTimer_.Start(changeFrame_);
	//}
}

void SceneTransitionManager::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();
	// デプスクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	// トランジション用のスプライト
	Sprite::PreDraw(commandList);

	transitionSprite_->SetColor(sColor);
	transitionSprite_->Draw();

	Sprite::PostDraw();
}
