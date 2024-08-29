#include "StateManager.h"
#include "../Player.h"
#include <cassert>

void StateManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	ChangeRequest(kIdleHorizontal, kHorizontal);
	ChangeRequest(kIdleVertical, kVertical);

}

void StateManager::ChangeRequest(StateList request, StateType type)
{
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

	switch (type)
	{
	case StateManager::kVertical:
		if (player_->GetVerticalState()) {
			player_->GetVerticalState()->Exit();
		}
		newState->PreInitialize(player_);
		newState->Initialize();
		player_->SetVerticalState(std::move(newState));
		break;
	case StateManager::kHorizontal:
		if (player_->GetHorizontalState()) {
			player_->GetHorizontalState()->Exit();
		}
		newState->PreInitialize(player_);
		newState->Initialize();
		player_->SetHorizontalState(std::move(newState));
		break;
	}
	//if (player_->GetState()) {
	//	player_->GetState()->Exit();
	//}
	//newState->PreInitialize(player_);
	//newState->Initialize();
	//player_->SetState(std::move(newState));

}
