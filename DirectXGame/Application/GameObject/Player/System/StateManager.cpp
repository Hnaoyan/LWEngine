#include "StateManager.h"
#include "../Player.h"
#include <cassert>

void StateManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;

}

void StateManager::Update()
{

	if (request_) {
		// 元の終了
		if (currentState_) {
			currentState_->Exit();
		}
		
		// 新しいやつの初期化
		tmpState_->PreInitialize(player_, this);
		tmpState_->Initialize();
		currentState_ = std::move(tmpState_);
		
		// リクエストリセット
		request_ = std::nullopt;
	}
	if (currentState_) {
		currentState_->InputHandle();
		currentState_->Update();
	}
}

void StateManager::ChangeRequest(StateList request)
{

	request_ = request;

	std::unique_ptr<IPlayerState> newState;
	switch (request)
	{
	case StateManager::kIdle:
		newState = std::make_unique<IdleState>();
		break;
	case StateManager::kIdleVertical:
		newState = std::make_unique<IdleVertical>();
		break;
	case StateManager::kIdleHorizontal:
		newState = std::make_unique<IdleHorizontal>();
		break;
	case StateManager::kMove:
		newState = std::make_unique<MovingState>();
		break;
	case StateManager::kJump:
		newState = std::make_unique<JumpingState>();
		break;
	case StateManager::kFall:
		newState = std::make_unique<FallingState>();
		break;
	case StateManager::kBoost:
		newState = std::make_unique<BoostState>();
		break;
	case StateManager::kQuickBoost:
		newState = std::make_unique<QuickBoostState>();
		break;
	case StateManager::kAssending:
		newState = std::make_unique<AssendingState>();
		break;
	}

	tmpState_ = std::move(newState);

}
