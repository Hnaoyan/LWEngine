#include "GameStateManager.h"
#include "GameTimerSystem.h"

void GameStateManager::Initialize()
{
	isClear_ = false;
}

void GameStateManager::Update()
{
	if (isClear_) {
		timer_->Pause();
	}
}
