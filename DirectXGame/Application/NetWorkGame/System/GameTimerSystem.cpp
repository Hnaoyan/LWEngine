#include "GameTimerSystem.h"
#include "Engine/LwLib/DeltaTime.h"
#include <imgui.h>

void GameTimerSystem::Initialize()
{
	// 開始
	Start();
}

void GameTimerSystem::Update()
{
	if (isActive_) {
		// フレーム加算
		elapsedFrame_ += kDeltaTime;
		// 整数型としてキャスト
		elapsedSecond_ = (float)int(elapsedFrame_);
	}
}

void GameTimerSystem::ImGuiDraw()
{
	ImGui::Begin("ゲームタイマー");

	ImGui::DragFloat("経過フレーム", &elapsedFrame_);
	ImGui::DragFloat("経過秒数", &elapsedSecond_);

	if (ImGui::Button("一時停止")) {
		Pause();
	}
	ImGui::SameLine();
	if (ImGui::Button("再開")) {
		Resume();
	}

	ImGui::End();
}

void GameTimerSystem::Pause()
{
	isActive_ = false;
}

void GameTimerSystem::Start()
{
	isActive_ = true;
	elapsedFrame_ = 0.0f;
	elapsedSecond_ = 0.0f;
}

void GameTimerSystem::Resume()
{
	isActive_ = true;
}