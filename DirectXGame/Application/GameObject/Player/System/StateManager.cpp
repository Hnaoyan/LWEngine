#include "StateManager.h"
#include "../Player.h"
#include <cassert>

void StateManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;
}

void StateManager::ChangeRequest(std::unique_ptr<IPlayerState> newState)
{
	if (player_->GetState()) {
		player_->GetState()->Exit();
	}
	newState->PreInitialize(player_);
	newState->Initialize();
	player_->SetState(std::move(newState));
}

void StateManager::ChangeRequest(StateList request)
{
	std::unique_ptr<IPlayerState> newState;
	switch (request)
	{
	case StateManager::kIdle:
		newState = std::make_unique<IdleState>();
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
	}

	if (player_->GetState()) {
		player_->GetState()->Exit();
	}
	newState->PreInitialize(player_);
	newState->Initialize();
	player_->SetState(std::move(newState));

}
