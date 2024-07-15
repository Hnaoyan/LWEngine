#include "StateMachine.h"
#include "../../Player.h"

#include <cassert>

void StateMachine::ChangeRequest(VerticalStates type)
{
	std::unique_ptr<IPlayerState> newState;
	switch (type)
	{
	case VerticalStates::kIdle:
		newState = std::make_unique<IdleState>();
		break;
	case VerticalStates::kJump:
		newState = std::make_unique<JumpingState>();
		break;
	case VerticalStates::kFall:
		newState = std::make_unique<FallingState>();
		break;
	}
	if (currentState_) {
		currentState_->Exit();
	}
	// 新しいステートの初期化
	newState->PreInitialize(player_, this);
	newState->Initialize();
	currentState_ = std::move(newState);
}

void StateMachine::ChangeRequest(HorizontalStates type)
{
	std::unique_ptr<IPlayerState> newState;
	switch (type)
	{
	case HorizontalStates::kIdle:
		newState = std::make_unique<IdleState>();
		break;
	case HorizontalStates::kMove:
		newState = std::make_unique<MovingState>();
		break;
	case HorizontalStates::kBoost:
		newState = std::make_unique<BoostState>();
		break;
	}
	if (currentState_) {
		currentState_->Exit();
	}
	// 新しいステートの初期化
	newState->PreInitialize(player_, this);
	newState->Initialize();
	currentState_ = std::move(newState);
}

void StateMachine::Initialize(Player* player, std::string name)
{
	assert(player);
	player_ = player;

	name_ = name;

	if (currentState_ == nullptr) {
		// 最初の初期化
		currentState_ = std::make_unique<IdleState>();
		currentState_->PreInitialize(player_, this);
		currentState_->Initialize();
	}

}

void StateMachine::Update()
{
	// 実体があるか
	if (currentState_) {
		// 更新フラグ
		currentState_->InputHandle();
		currentState_->Update();
	}
}
