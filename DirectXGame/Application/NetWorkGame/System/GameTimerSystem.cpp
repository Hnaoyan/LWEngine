#include "GameTimerSystem.h"
#include "Engine/LwLib/DeltaTime.h"
#include <imgui.h>

void GameTimerSystem::Initialize()
{
	// 初期化
	elapsedFrame_ = 0.0f;
	elapsedSecond_ = 0.0f;
}

void GameTimerSystem::Update()
{
	// フレーム加算
	elapsedFrame_ += kDeltaTime;
	// 整数型としてキャスト
	elapsedSecond_ = (float)int(elapsedFrame_);
}

void GameTimerSystem::ImGuiDraw()
{
	ImGui::Begin("ゲームタイマー");

	ImGui::DragFloat("経過フレーム", &elapsedFrame_);
	ImGui::DragFloat("経過秒数", &elapsedSecond_);

	ImGui::End();
}
