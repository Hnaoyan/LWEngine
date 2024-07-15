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
	newState->PreInitialize(player_, nullptr);
	newState->Initialize();
	player_->SetState(std::move(newState));
}

void StateManager::ChangeRequest(StateList request)
{
	std::unique_ptr<IPlayerState> newState;
	switch (request)
	{
	case StateList::kIdle:
		newState = std::make_unique<IdleState>();
		break;
	case StateList::kMove:
		newState = std::make_unique<MovingState>();
		break;
	case StateList::kJump:
		newState = std::make_unique<JumpingState>();
		break;
	case StateList::kFall:
		newState = std::make_unique<FallingState>();
		break;
	case StateList::kBoost:
		newState = std::make_unique<BoostState>();
		break;
	}

	if (player_->GetState()) {
		player_->GetState()->Exit();
	}
	newState->PreInitialize(player_, nullptr);
	newState->Initialize();
	player_->SetState(std::move(newState));

}
