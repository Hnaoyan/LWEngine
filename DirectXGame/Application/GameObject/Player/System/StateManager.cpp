#include "StateManager.h"
#include "../Player.h"
#include <cassert>

void StateManager::Initialize(Player* player)
{
	assert(player);
	player_ = player;

	ChangeRequest(kIdleHorizontal);
	ChangeRequest(kIdleVertical);

}

void StateManager::Update()
{

	if (request_) {

		std::unique_ptr<IPlayerState> newState = std::move(tmpState_);
		
		bool isVertical = false;
		bool isHorizontal = false;

		isVertical = (request_.value() == kIdleVertical) || (request_.value() == kAssending) ||
			(request_.value() == kFall) || (request_.value() == kJump);
		isHorizontal = (request_.value() == kIdleHorizontal) || (request_.value() == kMove) ||
			(request_.value() == kQuickBoost) || (request_.value() == kBoost);
		if (isVertical) {
			if (player_->GetVerticalState()) {
				player_->GetVerticalState()->Exit();
			}
			newState->PreInitialize(player_);
			newState->Initialize();
			player_->SetVerticalState(std::move(newState));
		}
		else if (isHorizontal) {
			if (player_->GetHorizontalState()) {
				player_->GetHorizontalState()->Exit();
			}
			newState->PreInitialize(player_);
			newState->Initialize();
			player_->SetHorizontalState(std::move(newState));
		}

		request_ = std::nullopt;
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

	//switch (type)
	//{
	//case StateManager::kVertical:
	//	if (player_->GetVerticalState()) {
	//		player_->GetVerticalState()->Exit();
	//	}
	//	newState->PreInitialize(player_);
	//	newState->Initialize();
	//	player_->SetVerticalState(std::move(newState));
	//	break;
	//case StateManager::kHorizontal:
	//	if (player_->GetHorizontalState()) {
	//		player_->GetHorizontalState()->Exit();
	//	}
	//	newState->PreInitialize(player_);
	//	newState->Initialize();
	//	player_->SetHorizontalState(std::move(newState));
	//	break;
	//}
}
