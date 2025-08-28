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