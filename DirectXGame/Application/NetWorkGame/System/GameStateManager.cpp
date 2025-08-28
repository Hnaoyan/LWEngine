#include "GameStateManager.h"
#include "GameTimerSystem.h"
#include "Network/LoginToken.h"

void GameStateManager::Initialize()
{
	isClear_ = false;
}

void GameStateManager::Update()
{
	if (isClear_) {
		timer_->Pause();

		if (isAccept_) {
			return;
		}
		isAccept_ = true;
	}

	if (isAccept_) {
		login_->ClearPost((int)timer_->GetElapsedSecond());
	}

	// リセット処理
	if (resetRequest_) {

	}
}
